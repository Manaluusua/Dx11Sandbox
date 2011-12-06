//sampler states etc
SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = MIRROR;
    AddressV = MIRROR;
	AddressW = MIRROR;
};

RasterizerState rsSolid { FillMode = Solid; };

//shader impl and uniforms


TextureCube cubemap; 

cbuffer sceneInfo
{
	float4x4 viewProj;
    float3	sunDirection;
	float3	sunColor;
};



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
    
    output.position = mul( float4(input.position,0), viewProj );
	
	//meh, far clip plane will clip if z is exactly 1, so need to make it a bit less. Anyway projecting depth to far clip plane.
	output.position.w = output.position.z +0.000001f;
	
    output.uv = input.uv;

    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
    

    float4 output = cubemap.Sample( samLinear, input.uv );
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

