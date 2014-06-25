
#define FRESNELSPECULAR fresnelSchlickSpecularWithRoughness
#define FRESNELDIFFUSE fresnelSchlickDiffuse
#define GEOMETRIC geometricTorranceSparrow
#define DISTRIBUTION distributionTrowbridgeReitz
#define DIFFUSE diffuseLambertian


#define PI 3.1415f
#define InvPI 1.f/PI

//geometric term
float geometricImplicit(float3 lightDir, float3 viewDir, float3 halfVec, float3 normal, float roughness)
{
	return 0.25f;
}

float geometricTorranceSparrow(float3 lightDir, float3 viewDir, float3 halfVec, float3 normal, float roughness)
{
	float dotNH = dot(normal, halfVec);
	float invDotVH = rcp(dot(viewDir, halfVec));
	float ret = min( (2.f * dotNH * dot(normal, viewDir)) * invDotVH, (2.f * dotNH * dot(normal, lightDir)) * invDotVH ); 
	return min( 1.f, ret ) * rcp(4.f * dot(normal,viewDir) * dot(normal, lightDir));
}

float geometricTorranceSparrowApprox(float3 lightDir, float3 viewDir, float3 halfVec, float3 normal, float roughness)
{
	float3 h = viewDir + halfVec;
	return 4.f * rcp(dot(h,h)) * 0.25f;
}

float GSchlick(float3 dir, float3 normal, float k)
{
	float dp = saturate(dot(dir, normal));
	return dp * rcp(dp * ( 1.f - k ) + k);
}

float geometricSmith(float3 lightDir, float3 viewDir, float3 halfVec, float3 normal, float roughness)
{
	float k = sqrt( (2.f * pow(roughness,2.f)) * InvPI );
	return GSchlick(lightDir, normal,k) * GSchlick(viewDir, normal,k) * rcp(4.f * dot(normal,viewDir) * dot(normal, lightDir));
}


//NDF
float distributionBlinnPhong(float3 halfVec, float3 normal, float roughness)
{
	float a = max(0.0001f, 2.f * rcp(roughness * roughness) - 2);
	float HdotN = min( saturate(dot(halfVec, normal)), 0.9999f );
	return ((roughness + 2) * 0.5f ) * pow( HdotN, a) ;
}

float distributionTrowbridgeReitz(float3 halfVec, float3 normal, float roughness)
{
	float r2 = roughness*roughness;
	float HdotN = min( saturate(dot(halfVec, normal)), 0.9999f );
	return r2 / pow(pow( HdotN, 2) * (r2 - 1.f) + 1, 2);
}



//fresnel
float3 fresnelSchlickSpecular(float3 spec,float3 viewDir ,float3 halfVector)
{
    return spec + (1.f - spec) * pow(1.0f - dot(viewDir, halfVector), 5);
}

float fresnelSchlickSpecular(float spec,float3 viewDir ,float3 halfVector)
{
    return spec + (1.f - spec) * pow(1.0f - dot(viewDir, halfVector), 5);
}



float3 fresnelSchlickDiffuse(float3 spec,float3 viewDir ,float3 normal )
{
	return spec + (1.f - spec) * pow(saturate(dot(viewDir, normal)), 5);
}

float fresnelSchlickDiffuse(float spec,float3 viewDir ,float3 normal )
{
	return spec + (1.f - spec) * pow(saturate(dot(viewDir, normal)), 5);
}

//taken from blog of Sébastien Lagarde:http://seblagarde.wordpress.com/2011/08/17/hello-world/
float3 fresnelSchlickSpecularWithRoughness(float3 spec,float3 viewDir ,float3 normal, float roughness )
{
	return spec + ( max(spec, (1.f - roughness) ) - spec) * pow(1.f - saturate(dot(viewDir, normal)), 5);
}

//misc

float3 diffuseLambertian(float3 albedo, float3 normal, float3 lightDir)
{
	return albedo * saturate(dot(lightDir, normal));
}

float luminance(float3 color)
{
 return (color.x + color.y + color.z) * 0.33333f;
}


float3 lightingEquation(float3 albedo, float3 lightColor, float3 specular,float roughness, float3 normal, float3 lightDir, float3 viewDir )
{
	roughness = max(roughness * roughness, 0.00001f);
	float3 halfVec = normalize(viewDir + lightDir);
	
	
	float3 specularBrdf =  FRESNELSPECULAR(specular, lightDir, halfVec, roughness)*GEOMETRIC(lightDir, viewDir, halfVec, normal, roughness)*DISTRIBUTION(halfVec, normal, roughness)* saturate(dot(lightDir, normal));
	float3 diffuseBrdf =(1.f - luminance(specular) ) * DIFFUSE(albedo, normal,lightDir);
	
	
	return (diffuseBrdf + specularBrdf)* lightColor;
}
