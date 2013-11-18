#include "Light.h"

namespace Dx11Sandbox
{
	Light::Light(void)
		:m_lightType(OMNI),
		m_color(1.f,1.f,1.f,1.f),
		m_params(0.f,0.f,0.f,0.f)

	{
	}
	Light::Light(LightType type, const D3DXVECTOR4& color,const D3DXVECTOR4& params )
		:m_lightType(type),
		m_color(color),
		m_params(params)
	{

	}

	Light::~Light(void)
	{
	}


	

	void Light::setColor( const D3DXVECTOR4& color )
	{
		m_color = color;
	}

	const D3DXVECTOR4& Light::getColor() const
	{
		return m_color;
	}

	void Light::setLightParameters(const D3DXVECTOR4& params)
	{
		m_params = params;
	}

	const D3DXVECTOR4& Light::getLightParameters() const
	{
		return m_params;
	}

	Light::LightType Light::getLightType() const
	{
		return m_lightType;
	}

	void Light::setLightType(Light::LightType type)
	{
		m_lightType = type;
	}
}