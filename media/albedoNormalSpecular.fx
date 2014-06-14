
SamplerState samLinear
{
    Filter = ANISOTROPIC;
    AddressU = WRAP;
    AddressV = WRAP;
};


#include "commonUniforms.fx"
#include "commonFunctions.hlsl"
#include "lightingEquations.hlsl"

Texture2D albedoTex;
Texture2D normalTex;
texture2D specularTex;  


struct VS_INPUT
{
    float3 position   : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
    float2 uv       : TEXCOORD0;
};


struct PS_INPUT_FORWARD
{
	float4 position : SV_POSITION;
    float2 uv   : TEXCOORD0;
	float3 camDir : TEXCOORD1;
	float3 lightDir : TEXCOORD2;
	float clipDistance: SV_ClipDistance0;
};

struct PS_INPUT_DEFERRED
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
    float2 uv   : TEXCOORD0;
	float clipDistance: SV_ClipDistance0;
};




PS_INPUT_FORWARD VS_Forward( VS_INPUT input )
{
    PS_INPUT_FORWARD output;
    float4 pos = mul( float4(input.position,1), worldviewProj );
	
    output.position = pos;
    output.uv = input.uv;
	output.clipDistance = dot(float4(input.position,1),clipPlane);
	
	float3 bitangent = cross( input.normal, input.tangent );
	
	float3x3 toTangentSpace = float3x3( input.tangent.x, input.normal.x, bitangent.x,
										input.tangent.y, input.normal.y, bitangent.y,
										input.tangent.z, input.normal.z, bitangent.z	);
	
	
	
	output.camDir = normalize( mul(camPos.xyz - pos, toTangentSpace) ) ;
	output.lightDir = normalize( mul(sunDirection, toTangentSpace) ) ;
	
	
    return output;
}

float4 PS_Forward( PS_INPUT_FORWARD input) : SV_Target
{
    
	float4 output = float4(0,0,0,1);
	
	float3 alb = albedoTex.Sample( samLinear, input.uv ).rgb;
	float4 spec = specularTex.Sample( samLinear, input.uv );
	float3 tangNorm = unpackNormal(normalTex.Sample( samLinear, input.uv ).rgb);

	output.rgb = lightingEquation(alb, sunColor, spec.rgb ,spec.w, tangNormal,input.lightDir, input.camDir );
    return output;
}


PS_INPUT_DEFERRED VS_Deferred( VS_INPUT input )
{
    PS_INPUT_DEFERRED output;
    
    output.position = mul( float4(input.position,1), worldviewProj );
	//for now, just assume non-uniform scale is not used and use same world transform as points
	output.normal = normalize( mul( float4(input.normal,0), world) );
	output.tangent =  normalize( mul( float4(input.tangent,0), world) );
    output.uv = input.uv;

	output.clipDistance = dot(float4(input.position,1),clipPlane);
	
    return output;
}

PS_GBUFFER_OUTPUT PS_Deferred( PS_INPUT_DEFERRED input) 
{
    
	PS_GBUFFER_OUTPUT output;
	
	output.color = albedoTex.Sample( samLinear, input.uv ).rgb;
	output.specular = specularTex.Sample( samLinear, input.uv );
	float3 normal = unpackNormal(normalTex.Sample( samLinear, input.uv ).rgb);
	
	float3 binormal = cross( input.normal, input.tangent );
	
	float3x3 toWorldSpace = float3x3( 
									input.tangent, 
									input.normal,
									binormal 
									);
	output.normal = normalize( mul( normal, toWorldSpace ) );
	
	
    return output;
}




//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 Forward
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_Forward() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Forward() ) );
    }
}
technique11 Deferred
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_Deferred() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Deferred() ) );
    }
}
