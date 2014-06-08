

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


#include "commonUniforms.fx"

float4 color;

struct VS_INPUT
{
    float3 position   : POSITION;
};


struct PS_INPUT
{
	float4 position : SV_POSITION;
};


PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
    output.position = mul( float4(input.position,1), worldviewProj );

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
    return color;
}




//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 gizmoColored
{
	
    pass P0
    {

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}