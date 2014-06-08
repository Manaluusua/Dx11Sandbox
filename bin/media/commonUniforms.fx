cbuffer basicSceneInfo
{
	float4x4 worldviewProj;
	float4x4 world;
	float3 camPos;
	float4 clipPlane;
	float time;
};

cbuffer objectInfo
{
	float3	sunDirection;
	float3	sunColor;
};


struct PS_GBUFFER_OUTPUT
{
    float4 color: SV_Target0;
    float4 normal: SV_Target1;
	float4 specular: SV_Target2;
};