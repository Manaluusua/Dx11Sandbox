//sampler states etc
SamplerState samLinear
{
    Filter =  MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


//shader impl and uniforms
static const float3 waterColor = float3(0.3f,0.6f,0.6f);
static const float4 specular = float4(1.0f,1.f,1.f,0.8f);

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

struct PS_INPUT_DEFERRED
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 uv   : TEXCOORD0;
	float4 reflPos :TEXCOORD1;
	float4 refrPos : TEXCOORD2;
	float3 camDir : TEXCOORD4;
}; 

struct PS_INPUT_FORWARD : PS_INPUT_DEFERRED
{
	float3 lightDir : TEXCOORD3;
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

float3x3 calculatePixelShaderInputDeferred(VS_INPUT input, out PS_INPUT_DEFERRED output)
{
	float3 wavePosition = input.position;
	
	float3 tangent = float3(0,0,0);
	float3 bitangent = float3( 0,0,0);
	
	calculateNormalTangentAndBitangentForPoint( wavePosition.xz,time, output.normal, tangent, bitangent );

	float3x3 toTangentSpace = float3x3( tangent.x, output.normal.x, bitangent.x,
										tangent.y, output.normal.y, bitangent.y,
										tangent.z, output.normal.z, bitangent.z	);
	
	
	wavePosition.y += calculateHeightForPoint(wavePosition.xz,time);
	
    output.position = mul( float4(wavePosition,1), worldviewProj );
	
	
    output.uv = input.uv;

	output.reflPos = mul(float4(input.position,1), reflectionViewProj );
	output.refrPos = mul( float4(input.position,1), refractionViewProj );
	
	
	output.camDir = normalize( mul(camPos.xyz - wavePosition, toTangentSpace) ) ;
	return toTangentSpace;
}

void calculatePixelShaderInputForward(VS_INPUT input, out PS_INPUT_FORWARD output)
{
	float3x3 toTangentSpace = calculatePixelShaderInputDeferred(input, (PS_INPUT_DEFERRED)output);
	output.lightDir = normalize( mul(sunDirection.xyz, toTangentSpace) );
}

void calculateWaterColorAndNormal(PS_INPUT_DEFERRED input, out float3 color, out float3 normal)
{
	float offsetRefl = 0.05;
	float offsetRefr = 0.0;
	
	float2 movedUvs = input.uv * 4;
	
	movedUvs.x += time * 0.08; 
	movedUvs.y += time * 0.08; 
	
	normal = input.normal;
	float3 newnormal = getNormalFromNormalMap(normalmap, movedUvs.xy*6);

	
	movedUvs.x -= time*0.08;
	movedUvs.y += time*0.08;
	float3 newnormal2 = getNormalFromNormalMap(normalmap, movedUvs.yx*2);
	
	
	normal = normal*0.2 + (newnormal + newnormal2)*0.4;
	
	
	

	float2 reflCoords = (input.reflPos.xy / input.reflPos.w)*0.5f+0.5f;
	reflCoords.y = -reflCoords.y;
	
	float2 refrCoords = (input.refrPos.xy / input.refrPos.w) *0.5f+0.5f;
	refrCoords.y = -refrCoords.y;
	
	
	
    float3 reflCol = reflection.Sample( samLinear, reflCoords + normal.xy*offsetRefl).rgb;
	float3 refrCol = refraction.Sample( samLinear, refrCoords + normal.xy*offsetRefr).rgb;
	
	color = waterColor * fresnel(normal, input.camDir, 1.0f, refrCol , reflCol);
	
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
	float3 color;
	
	calculateWaterColorAndNormal((PS_INPUT_DEFERRED)input, color, normal);
	output.rgb = color;

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
	float3 color;
	
	calculateWaterColorAndNormal(input, color, normal);
	output.color = float4(color, 1.f);
	output.normal.rgb = normal;
	output.specular = specular;
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
