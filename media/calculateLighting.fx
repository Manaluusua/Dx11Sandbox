//adapted from DICE whitepaper http://dice.se/wp-content/uploads/GDC11_DX11inBF3_Public.pdf

//global input

//gbuffer
Texture2D<float4> albedoTex : register(t0); 
Texture2D<float4> normalTex : register(t1); 
Texture2D<float4> specularTex : register(t2); 
Texture2D<float> depthTex : register(t3); 
 
RWTexture2D<float4> output : register(u0);

//lights
struct Light { 
 uint lightType;
 float3 pos; //float sqrRadius; 
 float3 color; //float invSqrRadius; 
}; 
 
StructuredBuffer<Light> lights; 



//misc
cbuffer misc : register(b0)
{
	float4x4 viewProj;
	float4x4 proj;
	float4 camPos;
	float4 screenDimensions;
	int lightCount;
};


 


//global internal
groupshared uint groupMinDepthInt; 
groupshared uint groupMaxDepthInt;


//functions
float2 calculateDepthMinMax(float depth)
{
	
	
	uint depthAsInt = asuint(depth); 
 
	groupMinDepthInt = 0xFFFFFFFF; 
	groupMaxDepthInt = 0; 
	GroupMemoryBarrierWithGroupSync(); 
 
	InterlockedMin(groupMinDepthInt, depthAsInt); 
	InterlockedMax(groupMaxDepthInt, depthAsInt); 

	GroupMemoryBarrierWithGroupSync(); 
	
	return float2(asfloat(groupMinDepthInt), asfloat(groupMaxDepthInt));
}


//main
#define GROUP_SIZE 16 
[numthreads(GROUP_SIZE,GROUP_SIZE,1)] 
void MainCS( 
 uint3 groupId : SV_GroupID, 
 uint3 groupThreadId : SV_GroupThreadID, 
 uint groupIndex: SV_GroupIndex, 
 uint3 dispatchThreadId : SV_DispatchThreadID) 
{ 
	
	//calculate group
	float depth = depthTex.Load(uint3(dispatchThreadId.xy, 0)).r;
	float2 minMaxDepth = calculateDepthMinMax(depth);
	
	float4 a = albedoTex.Load(uint3(dispatchThreadId.xy, 0)); 
	output[dispatchThreadId.xy] = a;
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

