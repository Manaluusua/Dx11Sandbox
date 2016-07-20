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

	

	void Light::setColor( const Vec3& color )
	{
		m_lightDefinition.colorInvRad[0] = color[0];
		m_lightDefinition.colorInvRad[1] = color[1];
		m_lightDefinition.colorInvRad[2] = color[2];
		lightParametersChanged();
	}

	const Vec4& Light::getColor() const
	{
		return m_lightDefinition.colorInvRad;
	}

	void Light::setPosition(const Vec3& pos)
	{
		
		m_lightDefinition.posRad[0] = pos[0];
		m_lightDefinition.posRad[1] = pos[1];
		m_lightDefinition.posRad[2] = pos[2];
		lightParametersChanged();
	}

	const Vec4& Light::getPosition() const
	{
		return m_lightDefinition.posRad;
	}

	void Light::setDirection(const Vec3& dir)
	{
		
		m_lightDefinition.dirAng[0] = dir[0];
		m_lightDefinition.dirAng[1] = dir[1];
		m_lightDefinition.dirAng[2] = dir[2];
		lightParametersChanged();
	}
	const Vec4& Light::getDirection() const
	{
		return m_lightDefinition.dirAng;
	}

	void Light::setRadius(float radius)
	{
		m_lightDefinition.posRad[3] = radius;
		m_lightDefinition.colorInvRad[3] = 1.f / (radius == 0.f ? 0.000001f : radius);
		lightParametersChanged();
	}
	float Light::getRadius() const
	{
		return m_lightDefinition.posRad[3];
	}

	void Light::setSpotlightAngle(float angle)
	{
		m_lightDefinition.dirAng[3] = angle;
		lightParametersChanged();
	}
	float Light::getSpotlightAngle()
	{
		return m_lightDefinition.dirAng[3];
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