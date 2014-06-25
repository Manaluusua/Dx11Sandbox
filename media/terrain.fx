//sampler states etc
SamplerState samLinear
{
    Filter = ANISOTROPIC;
    AddressU = WRAP;
    AddressV = WRAP;
};


//shader impl and uniforms

static const float3 ambient   = float3( 0.1f, 0.1f, 0.1f );  
static const float4 specular   = float4( 0.002f, 0.002f, 0.002f, 1.f );  
static const float3 heightLevels = float3(60.f,0,0);

#include "commonUniforms.fx"
#include "commonFunctions.hlsl"

Texture2D texture1;  

//Texture2D textureWeights;

struct VS_INPUT
{
    float3 position   : POSITION;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
    float2 uv : TEXCOORD0;
};


struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 uv   : TEXCOORD0;
	float clipDistance: SV_ClipDistance0;
};



PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
    output.position = mul( float4(input.position,1), worldviewProj );
	output.normal = mul( float4(input.normal,0), world);
	
    output.uv = input.uv;

	output.clipDistance = dot( mul( float4(input.position,1), world ),clipPlane);
	
    return output;
}

float4 PS_Forward( PS_INPUT input) : SV_Target
{
    
	float4 output = float4(0,0,0,1);
	
	output += texture1.Sample( samLinear, input.uv );
	
	
	
	output.rgb *= ambient + sunColor*saturate(dot(input.normal,-sunDirection));

	
	
    return output;
}

PS_GBUFFER_OUTPUT PS_Deferred( PS_INPUT input) 
{
    
	PS_GBUFFER_OUTPUT output;
	
	output.color = texture1.Sample( samLinear, input.uv );
	output.normal.rgb = packNormal( input.normal );
	output.specular = specular;
	output.environment = float4( ambient, 0.f );

    return output;
}




//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 Forward
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Forward() ) );
    }
}
technique11 Deferred
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_Deferred() ) );
    }
}
