//sampler states etc
SamplerState samLinear
{
    Filter =  MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


//shader impl and uniforms
static const float3 waterColor = float3(0.1f,0.2f,0.1f);
static const float3 ambient   = float3( 0.1f, 0.1f, 0.1f );
static const float4 specular = float4(1.0f,1.0f,1.0f,64.f);

cbuffer sceneInfo
{
	float4x4 viewProj;
    float3	sunDirection;
	float3	sunColor;
	float3 camPos;
	float4 clipPlane;
	float time;
};

cbuffer waterPlaneInfo
{
	float4x4 reflectionViewProj;

};

Texture2D reflection;  
Texture2D refraction;


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
	float4 reflPos :TEXCOORD1;
	float4 refrPos : TEXCOORD2;
	float3 halfVec : TEXCOORD3;
	float3 camDir : TEXCOORD4;
};

float3 fresnel(float3 normal, float3 viewDir, float3 col1, float3 col2)
{
	static const float mat1 = 1.0003f;
	static const float mat2 = 1.3333f;
	static const float exponent = 5;
	float a = abs((mat1 - mat2))/(mat1+mat2);
	return a*col1 + col2*(1-a)*pow(1-saturate(dot(normal, viewDir)),exponent);
}

PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
    output.position = mul( float4(input.position,1), viewProj );
	output.normal = input.normal;
	
    output.uv = input.uv;

	output.reflPos = mul(float4(input.position,1), reflectionViewProj );
	output.refrPos = output.position;
	
	output.camDir = normalize(camPos - input.position);
	
	
    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{
	float2 reflCoords = (input.reflPos.xy / input.reflPos.w)*0.5f+0.5f;
	reflCoords.y = -reflCoords.y;
	
	float2 refrCoords = (input.refrPos.xy / input.refrPos.w) *0.5f+0.5f;
	refrCoords.y = -refrCoords.y;
	
	float4 output = float4(0,0,0,1);
	
    float3 reflCol = reflection.Sample( samLinear, reflCoords ).rgb;
	float3 refrCol = refraction.Sample( samLinear, refrCoords ).rgb;
	
	output.rgb += fresnel(input.normal, input.camDir, refrCol + waterColor, reflCol);
	
	float3 halfVec = normalize(input.camDir + sunDirection);
	//output.rgb += pow(saturate(dot(halfVec, input.normal)), specular.w) * specular.rgb;
	
	
    return output;
}





//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 WaterPlane
{
	
    pass P0
    {

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

