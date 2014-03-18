#include "Light.h"

namespace Dx11Sandbox
{
	Light::Light(void)
	{
		memset(&m_lightDefinition, 0, sizeof(LightDefinition));
	}


	Light::~Light(void)
	{
	}

	

	void Light::setColor( const D3DXVECTOR3& color )
	{
		m_lightDefinition.colorInvRad.x = color.x;
		m_lightDefinition.colorInvRad.y = color.y;
		m_lightDefinition.colorInvRad.z = color.z;
		lightParametersChanged();
	}

	const D3DXVECTOR4& Light::getColor() const
	{
		return m_lightDefinition.colorInvRad;
	}

	void Light::setPosition(const D3DXVECTOR3& pos)
	{
		
		m_lightDefinition.posRad.x = pos.x;
		m_lightDefinition.posRad.y = pos.y;
		m_lightDefinition.posRad.z = pos.z;
		lightParametersChanged();
	}

	const D3DXVECTOR4& Light::getPosition() const
	{
		return m_lightDefinition.posRad;
	}

	void Light::setDirection(const D3DXVECTOR3& dir)
	{
		
		m_lightDefinition.dirAng.x = dir.x;
		m_lightDefinition.dirAng.y = dir.y;
		m_lightDefinition.dirAng.z = dir.z;
		lightParametersChanged();
	}
	const D3DXVECTOR4& Light::getDirection() const
	{
		return m_lightDefinition.dirAng;
	}

	void Light::setRadius(float radius)
	{
		m_lightDefinition.posRad.w = radius;
		m_lightDefinition.colorInvRad.w = 1.f / (radius == 0.f ? 0.000001f : radius);
		lightParametersChanged();
	}
	float Light::getRadius() const
	{
		return m_lightDefinition.posRad.w;
	}

	void Light::setSpotlightAngle(float angle)
	{
		m_lightDefinition.dirAng.w = angle;
		lightParametersChanged();
	}
	float Light::getSpotlightAngle()
	{
		return m_lightDefinition.dirAng.w;
	}

	Light::LightType Light::getLightType() const
	{
		return static_cast<LightType>(m_lightDefinition.lightType);
	}

	void Light::setLightType(Light::LightType type)
	{
		m_lightDefinition.lightType = type;
		lightParametersChanged();
	}

	void Light::lightParametersChanged()
	{

	}
}