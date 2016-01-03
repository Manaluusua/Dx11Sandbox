#ifndef DX11SANDBOX_LIGHT_H
#define DX11SANDBOX_LIGHT_H
#include "CommonTypes.h"
#include "CommonMath.h"

namespace Dx11Sandbox
{

	struct LightDefinition {
		uint32_t lightType;
		Vec4 colorInvRad;
		Vec4 posRad;
		Vec4 dirAng;
	};

	class Light
	{
	public:

		enum LightType
		{
			OMNI = 0,
			DIRECTIONAL = 1,
			SPOT = 2
		};

		Light(void);
		virtual ~Light(void);

		void setColor( const Vec3& color );
		const Vec4& getColor() const;

		void setPosition(const Vec3& pos);
		const Vec4& getPosition() const;

		void setDirection(const Vec3& dir);
		const Vec4& getDirection() const;

		void setRadius(float radius);
		float getRadius() const;

		void setSpotlightAngle(float angle);
		float getSpotlightAngle();

		LightType getLightType() const;
		virtual void setLightType(LightType type);

		const LightDefinition& getLightDefinition() const;

	protected:

		virtual void lightParametersChanged();
		LightDefinition m_lightDefinition;
	};


	inline const LightDefinition& Light::getLightDefinition() const
	{
		return m_lightDefinition;
	}

};

#endif