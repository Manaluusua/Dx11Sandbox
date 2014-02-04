cbuffer sceneInfo
{
	float4x4 worldviewProj;
    float3	sunDirection;
	float3	sunColor;
	float3 camPos;
	float4 clipPlane;
	float time;
};
