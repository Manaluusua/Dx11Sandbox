//sampler states etc
SamplerState samLinear
{
    Filter =  MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


//shader impl and uniforms
static const float3 waterColor = float3(0.5f,0.5f,0.5f);
static const float4 specular = float4( 0.3, 0.3, 0.3,0.1f);

static const float offsetRefl = 0.05;
static const float offsetRefr = 0.0;


Texture2D refraction;
Texture2D reflection;  
Texture2D normalmap;

#include "commonUniforms.fx"
#include "commonFunctions.hlsl"
#include "lightingEquations.hlsl"

cbuffer waterPlaneInfo
{
	float4x4 reflectionViewProj;
	float4x4 refractionViewProj;
};

cbuffer waveDefinitions
{
	float4x4 waves;
};


struct VS_INPUT
{
    float3 position   : POSITION;
	float3 normal : NORMAL;
    float2 uv       : TEXCOORD0;
};

struct PS_INPUT_DEFERRED
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
    float2 uv   : TEXCOORD0;
	float4 reflPos :TEXCOORD1;
	float4 refrPos : TEXCOORD2;
	float3 camDir : TEXCOORD4;
}; 

struct PS_INPUT_FORWARD
{
	float4 position : SV_POSITION;
    float2 uv   : TEXCOORD0;
	float4 reflPos :TEXCOORD1;
	float4 refrPos : TEXCOORD2;
	float3 camDir : TEXCOORD4;
	float3 lightDir : TEXCOORD3;
};





float calculateHeightForPoint(float2 position, float time)
{
	float height = 0;
	height += waves._14*sin( dot(waves._11_12, position) * waves._13 + time*waves._41);
	
	height += waves._24*sin( dot(waves._21_22, position) * waves._23 + time*waves._42);
	
	height += waves._34*sin( dot(waves._31_32, position) * waves._33 + time*waves._43);
	
	return height;
}

void calculateNormalTangentAndBitangentForPoint(in float2 position,in float time, out float3 normal, out float3 tangent, out float3 bitangent)
{
	//calculate derivatives for xz and construct tangent, bitangent and normal from these
	float2 derivatives = waves._13 * waves._11_12 * waves._14*cos( dot(waves._11_12, position) * waves._13 + time*waves._41);
	
	derivatives += waves._23 * waves._21_22 * waves._24*cos( dot(waves._21_22, position) * waves._23 + time*waves._42);
	
	derivatives += waves._33 * waves._31_32 * waves._34*cos( dot(waves._31_32, position) * waves._33 + time*waves._43);
	
	normal = float3( -derivatives.x, 1, -derivatives.y );
	tangent = float3( 0, derivatives.y, 1 );
	bitangent = float3( 1, derivatives.x, 0 );
	
	normal = normalize( normal );
	tangent = normalize( tangent );
	bitangent = normalize( bitangent );
}



float3 getNormalFromNormalMap( Texture2D normalmap, float2 uvs )
{
	float3 normal = normalmap.Sample( samLinear, uvs ).rbg;
	normal = (normal - 0.5) * 2;
	
	return normalize(normal);
}

void calculatePixelShaderInputDeferred(VS_INPUT input, out PS_INPUT_DEFERRED output)
{
	float3 wavePosition = input.position;
	
	float3 tangent;
	float3 bitangent;
	float3 normal;
	
	calculateNormalTangentAndBitangentForPoint( wavePosition.xz,time, normal, tangent, bitangent );
	
	wavePosition.y += calculateHeightForPoint(wavePosition.xz,time);
	
    output.position = mul( float4(wavePosition,1), worldviewProj );
	float4 worldPos = mul( float4(wavePosition,1), world );
	
    output.uv = input.uv;

	output.reflPos = mul(float4(input.position,1), reflectionViewProj );
	output.refrPos = mul( float4(input.position,1), refractionViewProj );
	
	output.normal = normalize( mul(float4(normal, 0.f), world).xyz );
	output.bitangent = normalize( mul(float4(bitangent, 0.f), world).xyz );
	output.tangent = normalize( mul(float4(tangent, 0.f), world).xyz );
	
	
	
	
	output.camDir = normalize(camPos.xyz - worldPos.xyz);

}

void calculatePixelShaderInputForward(VS_INPUT input, out PS_INPUT_FORWARD output)
{
	float3 wavePosition = input.position;
	
	float3 tangent;
	float3 bitangent;
	float3 normal;
	
	calculateNormalTangentAndBitangentForPoint( wavePosition.xz,time, normal, tangent, bitangent );

	float3x3 toTangentSpace = float3x3( tangent.x, normal.x, bitangent.x,
										tangent.y, normal.y, bitangent.y,
										tangent.z, normal.z, bitangent.z	);
	
	
	wavePosition.y += calculateHeightForPoint(wavePosition.xz,time);
	
    output.position = mul( float4(wavePosition,1), worldviewProj );
	float4 worldPos = mul( float4(wavePosition,1), world );
	
    output.uv = input.uv;

	output.reflPos = mul(float4(input.position,1), reflectionViewProj );
	output.refrPos = mul( float4(input.position,1), refractionViewProj );
	
	output.camDir = normalize( mul( mul(float4( camPos.xyz - worldPos.xyz ,0.f) , worldInv).xyz, toTangentSpace ) ) ;
	output.lightDir = normalize( mul( mul(float4(-sunDirection, 0.f), worldInv).xyz, toTangentSpace ) ) ;
	

}


void calculateWaterNormal(float2 uv, out float3 normal)
{
	
	uv.x += time * 0.012; 
	uv.y += time * 0.01; 
	
	
	float3 newnormal1 = getNormalFromNormalMap(normalmap, uv.xy*8).rgb;

	
	uv.x -= time*0.01;
	uv.y += time*0.05;
	float3 newnormal2 = getNormalFromNormalMap(normalmap, uv.yx*2);
	
	
	normal = normalize(newnormal1 + newnormal2);
}

void calculateWaterColors(float4 reflPos, float4 refrPos, out float3 reflColor, out float3 refrColor, float3 normal)
{
	
	
	float2 reflCoords = (reflPos.xy / reflPos.w)*0.5f+0.5f;
	reflCoords.y = -reflCoords.y;
	
	float2 refrCoords = (refrPos.xy / refrPos.w) *0.5f+0.5f;
	refrCoords.y = -refrCoords.y;
	
	
	
    reflColor = reflection.Sample( samLinear, reflCoords + normal.xy*offsetRefl).rgb;
	refrColor = refraction.Sample( samLinear, refrCoords + normal.xy*offsetRefr).rgb;
	
	
	
}





//forward
PS_INPUT_FORWARD VS_FORWARD( VS_INPUT input )
{
    PS_INPUT_FORWARD output;

	calculatePixelShaderInputForward(input, output);

    return output;
}

float4 PS_FORWARD( PS_INPUT_FORWARD input) : SV_Target
{
	float4 output = float4(0,0,0,1);
	float3 normal;
	float3 colorRefl;
	float3 colorRefr;

	calculateWaterNormal(input.uv, normal);
	calculateWaterColors(input.reflPos,input.refrPos, colorRefl, colorRefr, normal);
	
	
	
	float f = fresnelSchlickDiffuse(specular.w,input.camDir ,normal );
	output.rgb =  f * colorRefr * waterColor + (1.f-f) * colorRefl;

	float3 halfVec = normalize(input.camDir + input.lightDir);
	output.rgb += pow(saturate(dot(halfVec, normal)), specular.w) * specular.rgb;
	
    return output;
}

//deferred
PS_INPUT_DEFERRED VS_DEFERRED( VS_INPUT input )
{
    PS_INPUT_DEFERRED output;

	calculatePixelShaderInputDeferred(input, output);

    return output;
}

PS_GBUFFER_OUTPUT PS_DEFERRED( PS_INPUT_DEFERRED input) 
{

	PS_GBUFFER_OUTPUT output;
	float3 normal;
	float3 colorRefl;
	float3 colorRefr;
	
	float3x3 toWorldSpace = float3x3( 
									input.tangent, 
									input.normal,
									input.bitangent
									);
	
	calculateWaterNormal(input.uv, normal);
	
	normal = mul(normal, toWorldSpace);
	
	
	calculateWaterColors(input.reflPos,input.refrPos,colorRefl, colorRefr, normal);
	
	float3 f = fresnelSchlickSpecularWithRoughness(specular.rgb,input.camDir ,normal, specular.w);
	
	output.color = float4(colorRefr * (1.f - f) * waterColor, 1.f);
	output.normal.rgb = packNormal(normal);
	output.specular = specular;
	output.environment = float4( colorRefl, 1.f );
    return output;
}



//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 Forward
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_FORWARD() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_FORWARD() ) );
    }
}
technique11 Deferred
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_DEFERRED() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_DEFERRED() ) );
    }
}
