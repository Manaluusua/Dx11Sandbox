
#define FRESNELSPECULAR fresnelSchlickSpecular
#define FRESNELDIFFUSE fresnelDiffuse
#define GEOMETRIC geometricImplicit
#define DISTRIBUTION distributionBlinnPhong
#define DIFFUSE diffuseLambertian


#define PI 3.1415

float geometricImplicit(float3 lightDir, float3 viewDir, float3 halfVec, float3 normal, float roughness)
{
	return 0.25f;
}

float distributionBlinnPhong(float3 halfVec, float3 normal, float roughness)
{


	float a = max(2 * rcp(roughness * roughness) - 2, 0.001f);
	return ((a + 2) * 0.5f ) * pow(saturate(dot(halfVec, normal)), a);
}

float3 fresnelSchlickSpecular(float3 refraction,float3 lightDir ,float3 halfVector)
{
    return refraction + (1.f - refraction) * pow(1.0f - saturate(dot(lightDir, halfVector)), 5);
}

float3 fresnelDiffuse(float3 refraction,float3 lightDir ,float3 normal )
{
	return refraction + (1.f - refraction) * pow(saturate(dot(lightDir, normal)), 5);
}

float3 diffuseLambertian(float3 albedo, float3 normal, float3 lightDir)
{
	return albedo * saturate(dot(lightDir, normal));
}

float3 lightingEquation(float3 albedo, float3 lightColor, float3 specular,float roughness, float3 normal, float3 lightDir, float3 viewDir )
{
	roughness = max(roughness, 0.001f);
	float3 halfVec = normalize(viewDir + lightDir);
	float3 specularBrdf = FRESNELSPECULAR(specular, lightDir, halfVec)*GEOMETRIC(lightDir, viewDir, halfVec, normal, roughness)*DISTRIBUTION(halfVec, normal, roughness) * saturate(dot(lightDir, normal));
	float3 diffuseBrdf = (1.f - FRESNELDIFFUSE(specular, lightDir, normal))*DIFFUSE(albedo, normal,lightDir);
	
	//scaling with fresnel to balance ratios between specular and diffuse is not really physically correct way to retain energy conservation since it violates reciprocity of the equation but haven't found a better way yet
	return (diffuseBrdf + specularBrdf)* lightColor;

	/*float3 color = albedo * saturate(dot(lightDir, normal)) * lightColor;
	
	color += pow(saturate(dot(halfVec, normal)), roughness * 1024) * specular;
	return color;
	*/
}