//sampler states etc
SamplerState samLinear
{
    Filter =  MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


//shader impl and uniforms
static const float3 waterColor = float3(0.3f,0.6f,0.6f);
static const float4 specular = float4(1.0f,0.7f,0.4f,64.f);

Texture2D refraction;
Texture2D reflection;  
Texture2D normalmap;

#include "commonUniforms.fx"

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


struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 uv   : TEXCOORD0;
	float4 reflPos :TEXCOORD1;
	float4 refrPos : TEXCOORD2;
	float3 lightDir : TEXCOORD3;
	float3 camDir : TEXCOORD4;
};

float3 fresnel(float3 normal, float3 viewDir, float exponent, float3 col1, float3 col2)
{
	static const float mat1 = 1.0003f;
	static const float mat2 = 1.3333f;

	float a = abs((mat1 - mat2))/(mat1+mat2);
	return a*col1 + col2*(1-a)*pow(1-saturate(dot(normal, viewDir)),exponent);
}

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
	normal.b = -normal.b;
	normal.r = -normal.r;
	
	return normalize(normal);
}



PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output;
    
	float3 wavePosition = input.position;
	
	float3 tangent = float3(0,0,0);
	float3 bitangent = float3( 0,0,0);
	
	calculateNormalTangentAndBitangentForPoint( wavePosition.xz,time, output.normal, tangent, bitangent );

	
	wavePosition.y += calculateHeightForPoint(wavePosition.xz,time);
	
    output.position = mul( float4(wavePosition,1), worldviewProj );
	
	
    output.uv = input.uv;

	output.reflPos = mul(float4(input.position,1), reflectionViewProj );
	output.refrPos = mul( float4(input.position,1), refractionViewProj );
	
	float3x3 toTangentSpace = float3x3( tangent.x, output.normal.x, bitangent.x,
										tangent.y, output.normal.y, bitangent.y,
										tangent.z, output.normal.z, bitangent.z	);
	output.camDir = normalize( mul(camPos.xyz - wavePosition, toTangentSpace) ) ;
	output.lightDir = normalize( mul(sunDirection.xyz, toTangentSpace) );
	
	
	
    return output;
}

float4 PS( PS_INPUT input) : SV_Target
{

	float offsetRefl = 0.05;
	float offsetRefr = 0.0;
	
	float2 movedUvs = input.uv * 4;
	
	movedUvs.x += time * 0.08; 
	movedUvs.y += time * 0.08; 
	
	float3 normal = input.normal;
	float3 newnormal = getNormalFromNormalMap(normalmap, movedUvs.xy*6);

	
	movedUvs.x -= time*0.08;
	movedUvs.y += time*0.08;
	float3 newnormal2 = getNormalFromNormalMap(normalmap, movedUvs.yx*2);
	
	
	normal = normal*0.2 + (newnormal + newnormal2)*0.4;
	
	
	

	float2 reflCoords = (input.reflPos.xy / input.reflPos.w)*0.5f+0.5f;
	reflCoords.y = -reflCoords.y;
	
	float2 refrCoords = (input.refrPos.xy / input.refrPos.w) *0.5f+0.5f;
	refrCoords.y = -refrCoords.y;
	
	float4 output = float4(0,0,0,1);
	
    float3 reflCol = reflection.Sample( samLinear, reflCoords + normal.xy*offsetRefl).rgb;
	float3 refrCol = refraction.Sample( samLinear, refrCoords + normal.xy*offsetRefr).rgb;
	
	output.rgb += waterColor * fresnel(normal, input.camDir, 1.0f, refrCol , reflCol);

	float3 halfVec = normalize(input.camDir + input.lightDir);
	output.rgb += pow(saturate(dot(halfVec, normal)), specular.w) * specular.rgb;
	
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

