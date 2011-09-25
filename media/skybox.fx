
static const float3 ambient   = float3( 0.0f, 0.0f, 0.0f );  

TextureCube cubemap; 

matrix worldViewProj;


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;
	AddressW = MIRROR;
};

RasterizerState rsSolid { FillMode = Solid; };

struct VS_INPUT
{
    float3 position   : POSITION;
    float3 uv       : TEXCOORD0;
};


struct PS_INPUT
{
	float4 position : SV_POSITION;
    float3 uv   : TEXCOORD0;
};


PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
    output.position = mul( float4(input.position,0), worldViewProj );
	
	//meh, far clip plane will clip if z is exactly 1, so need to make it a bit less. Anyway projecting depth to far clip plane.
	output.position.w = output.position.z +0.000001f;
	
    output.uv = input.uv;

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
    

    float4 output = cubemap.Sample( samLinear, input.uv );
	output.rgb += ambient;
    return output;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 SkyBox
{
	
    pass P0
    {

		SetRasterizerState( rsSolid );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

