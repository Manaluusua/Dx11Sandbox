float3 unpackNormal(float3 normal)
{
	return (normal - 0.5f) * 2.f;
}

float3 packNormal(float3 normal)
{
	return (normal * 0.5f) + 0.5;
}
