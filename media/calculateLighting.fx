//adapted from DICE  http://dice.se/wp-content/uploads/GDC11_DX11inBF3_Public.pdf and Andrew Lauritzen: http://software.intel.com/en-us/articles/deferred-rendering-for-current-and-future-rendering-pipelines/

#include "lightingEquations.hlsl"
#include "commonfunctions.hlsl"

#define GROUP_DIMENSION 16
#define GROUP_SIZE GROUP_DIMENSION * GROUP_DIMENSION

#define LIGHT_TYPE_OMNI 0
#define LIGHT_TYPE_DIRECTIONAL 1
#define LIGHT_TYPE_SPOT 2

//global input

//gbuffer
Texture2D<float4> albedoTex : register(t0); 
Texture2D<float4> normalTex : register(t1); 
Texture2D<float4> specularTex : register(t2);
Texture2D<float4> environmentTex : register(t3); 
Texture2D<float> depthTex : register(t4); 
 
RWTexture2D<float4> output : register(u0);

//lights
struct Light { 
	uint lightType;
	float4 colorInvRad; 
	float4 posRad;
	float4 dirAng;
 };
 
StructuredBuffer<Light> lights; 




//misc
cbuffer misc : register(b0)
{
	float4x4 invViewMat;
	float4x4 viewMat;
	float4x4 projMat;
	float4 camPos;
	float4 screenDimensions;
	uint lightCount;
};


 


//group global internal
groupshared uint groupMinDepthInt; 
groupshared uint groupMaxDepthInt;

groupshared uint visibleLightCount; 
groupshared uint visibleLightIndices[1024]; 


//internal
struct CullInfo { 
	float4 frustumPlanes[6]; 
 };
 
struct GBufferSample {
	float4 specular;
	float3 albedo; float bufferDepth;
	float3 normal; float viewDepth;
	float4 environment;
};


//functions

float3 calculateShadingSpotLight(float3 pos, float3 camDir, GBufferSample sample, Light light)
{
	return float3(0.f, 0.f, 0.f);
}

float3 calculateShadingOmniLight(float3 pos, float3 camDir, GBufferSample sample, Light light)
{
	float3 lightDir = light.posRad.xyz - pos;
	float lightDist = length(lightDir);
	lightDir = lightDir * rcp(lightDist);
	
	float atten = clamp(1.0f - light.colorInvRad.w * lightDist, 0.0, 1.0);
	
	float3 color = lightingEquation(sample.albedo, light.colorInvRad.rgb, sample.specular.rgb, sample.specular.w, sample.normal, lightDir, camDir);
	
	color *= atten;
	return color;
}

float3 calculateShadingDirectionalLight(float3 pos, float3 camDir, GBufferSample sample, Light light)
{


	float3 lightDir = -light.dirAng.xyz;
	
	float3 color = lightingEquation(sample.albedo, light.colorInvRad.rgb, sample.specular.rgb, sample.specular.w, sample.normal, lightDir, camDir);
	
	return color;
	
}

float3 calculateAmbientLighting(float3 pos, float3 camDir, GBufferSample sample) 
{
	return fresnelSchlickSpecularWithRoughness(sample.specular.rgb, camDir, sample.normal, sample.specular.a) * sample.environment.rgb * sample.environment.a;
}

float3 calculateShading(float2 threadIdXY, GBufferSample sample)
{
	
	float2 projxy = ( (threadIdXY) * float2(screenDimensions.z, screenDimensions.w)) * 2.f  - 1.f;
	projxy.y = -projxy.y;
	
	
	float2 scaledScrnPos = float2(projxy.x / projMat._11, projxy.y / projMat._22);
    
	float vd = sample.viewDepth;
    float4 viewPos = float4(scaledScrnPos.x * vd, scaledScrnPos.y * vd, vd, 1.f);
	
	viewPos = mul( viewPos, invViewMat );
	float3 wpos = viewPos.xyz;
	
	float3 camDir = camPos.xyz - wpos;
	camDir = normalize(camDir);
	
	float3 color = float3(0.f, 0.f, 0.f);
	for (uint index = 0; index < visibleLightCount; ++index) {
        Light light = lights[visibleLightIndices[index]];
		
		
        if( light.lightType == LIGHT_TYPE_OMNI )
		{
			color += calculateShadingOmniLight(wpos, camDir, sample, light);
		} else if( light.lightType == LIGHT_TYPE_DIRECTIONAL )
		{
			color += calculateShadingDirectionalLight(wpos, camDir, sample, light);
		} else if( light.lightType == LIGHT_TYPE_SPOT )
		{
			color += calculateShadingSpotLight(wpos, camDir, sample, light);
		}
		
		
    }
	
	color += calculateAmbientLighting(wpos, camDir, sample);
	return color;

}

void sampleGBuffer(uint2 sampleLocation, int sampleIndex, out GBufferSample sample){
	float zBuffDepth = depthTex.Load(uint3(sampleLocation, 0)).r;
	float viewDepth = projMat._43 / (zBuffDepth - projMat._33);
	
	sample.bufferDepth = zBuffDepth;
	sample.viewDepth = viewDepth;
	sample.albedo = albedoTex.Load(uint3(sampleLocation, 0)).xyz;
	sample.normal = unpackNormal( normalTex.Load(uint3(sampleLocation, 0)).xyz );
	sample.specular = specularTex.Load(uint3(sampleLocation, 0));
	sample.environment = environmentTex.Load(uint3(sampleLocation, 0));
}

void cullLights(uint groupIndex, CullInfo info)
{
	 
    for (uint index = groupIndex; index < lightCount; index += GROUP_SIZE) {
        Light light = lights[index];
        
		bool IsVisible = true;
		
		if( light.lightType == LIGHT_TYPE_OMNI )
		{
			//cull omnilight
			float4 posRad = light.posRad;
			float4 lightViewPos = mul( float4(posRad.xyz, 1.f) , viewMat);
			
			[unroll] for (uint i = 0; i < 6; ++i) {
				float d = dot(info.frustumPlanes[i], lightViewPos);
				IsVisible = IsVisible && (d >= -posRad.w);
			}
		} else if( light.lightType == LIGHT_TYPE_SPOT )
		{
			//not supported atm
			IsVisible = false;
		} 
		
		//directional light is always inside, no culling needed
        

        [branch] if (IsVisible) {
            //if light was visible, add it's index to visible list
            uint nextVisibleListIndex;
            InterlockedAdd(visibleLightCount, 1, nextVisibleListIndex);
            visibleLightIndices[nextVisibleListIndex] = index;
        }
    }

    GroupMemoryBarrierWithGroupSync();
	
}

void calculateCullInfo(float depth, uint2 groupIdXY, out CullInfo info )
{
	uint depthAsInt = asuint(depth); 
		
	GroupMemoryBarrierWithGroupSync(); 
 
	InterlockedMin(groupMinDepthInt, depthAsInt); 
	InterlockedMax(groupMaxDepthInt, depthAsInt); 

	GroupMemoryBarrierWithGroupSync(); 
	
	float2 minMaxDepth = float2(asfloat(groupMinDepthInt), asfloat(groupMaxDepthInt));
	
	// scale and bias projection matrix parameters to extract frustrum planes for tile frustum 
    float2 scale = float2(screenDimensions.xy) * rcp(float(GROUP_DIMENSION * 2));
    float2 bias = scale - float2(groupIdXY);
	
	//extract scaled and biased projection values (only columns 1,2 and 4 needed since near & far planes are taken from min/max depth values
    float4 col1 = float4(projMat._11 * scale.x, 0.0f, bias.x, 0.0f);
    float4 col2 = float4(0.0f, -projMat._22 * scale.y, bias.y, 0.0f);
    float4 col4 = float4(0.0f, 0.0f, 1.0f, 0.0f);

    info.frustumPlanes[0] = col4 - col1;
    info.frustumPlanes[1] = col4 + col1;
    info.frustumPlanes[2] = col4 - col2;
    info.frustumPlanes[3] = col4 + col2;
	
    // near and far plane can be directly calculated from depths since we are calculating view space frust
    info.frustumPlanes[4] = float4(0.0f, 0.0f,  1.0f, -minMaxDepth.x);
    info.frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f,  minMaxDepth.y);
    
    [unroll] for (uint i = 0; i < 4; ++i) {
        info.frustumPlanes[i] *= rcp(length(info.frustumPlanes[i].xyz));
    }
}


//main

[numthreads(GROUP_DIMENSION,GROUP_DIMENSION,1)] 
void MainCS( 
 uint3 groupId : SV_GroupID, 
 uint3 groupThreadId : SV_GroupThreadID, 
 uint groupIndex: SV_GroupIndex, 
 uint3 dispatchThreadId : SV_DispatchThreadID) 
{ 
	//init values
	if (groupIndex == 0) 
	{
		visibleLightCount = 0;
		groupMinDepthInt = 0x7F7FFFFF; 
		groupMaxDepthInt = 0; 
	}
	
	GBufferSample gSample;
	sampleGBuffer(dispatchThreadId.xy, 0, gSample);
	
	
	
	CullInfo cullInfo;
	calculateCullInfo( gSample.viewDepth, groupId.xy, cullInfo );
	
	
	cullLights(groupIndex, cullInfo);
	
	float3 color = calculateShading(dispatchThreadId.xy, gSample);
	
	output[dispatchThreadId.xy] = float4(color, 1.f);
} 




//Tech & Pass 
technique11 UnlitTextureScreen
{
	
    pass P0
    {

        SetVertexShader( NULL );
        SetGeometryShader( NULL );
        SetPixelShader( NULL );
		SetComputeShader( CompileShader( cs_5_0, MainCS() ) );
    }
}

