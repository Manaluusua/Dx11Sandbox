//sampler states etc
SamplerState samLinear
{
    Filter = ANISOTROPIC;
    AddressU = WRAP;
    AddressV = WRAP;
};



//shader impl and uniforms

static const float3 ambient   = float3( 0.1f, 0.1f, 0.1f );  

cbuffer sceneInfo
{
	float4x4 viewProj;
    float3	sunDirection;
	float3	sunColor;
	float3 camPos;
	float4 clipPlane;
	float time;
};

Texture2D texture1;  



struct VS_INPUT
{
    float3 position   : POSITION;
	float3 normal : NORMAL;
    float2 uv       : TEXCOORD0;
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
    
    output.position = mul( float4(input.position,1), viewProj );
	output.normal = input.normal;
	
    output.uv = input.uv;
	
	output.clipDistance = dot(float4(input.position,1),clipPlane);
	
    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
    
	
    float4 output = texture1.Sample( samLinear, input.uv );
	output.rgb *= ambient + sunColor*saturate(dot(input.normal,sunDirection));

	
	
    return output;
}





//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 Terrain
{
	
    pass P0
    {
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

