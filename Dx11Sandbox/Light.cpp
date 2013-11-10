#include "Light.h"


namespace Dx11Sandbox
{
	Light::Light(void)
	{
	}


	Light::~Light(void)
	{
	}

	CullableType Light::GetCullableType() const
	{
		return CULLABLE_LIGHT;
	}

};
