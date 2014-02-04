

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


#include "commonUniforms.fx"

Texture2D tex; 



struct VS_INPUT
{
    float3 position   : POSITION;
    float2 uv       : TEXCOORD0;
};


struct PS_INPUT
{
	float4 position : SV_POSITION;
    float2 uv   : TEXCOORD0;
};


PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
    output.position = mul( float4(input.position,1), worldviewProj );
	
    output.uv = input.uv;

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
    

    float4 output = tex.Sample( samLinear, input.uv );
    return output;
}




//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 UnlitTextureScreen
{
	
    pass P0
    {

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

