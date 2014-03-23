#ifndef DX11SANDBOX_LIGHT_H
#define DX11SANDBOX_LIGHT_H
#include "CommonTypes.h"
#include <d3dx9math.h>

namespace Dx11Sandbox
{

	struct LightDefinition {
		UINT lightType;
		D3DXVECTOR4 colorInvRad;
		D3DXVECTOR4 posRad;
		D3DXVECTOR4 dirAng;
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

		void setColor( const D3DXVECTOR3& color );
		const D3DXVECTOR4& getColor() const;

		void setPosition(const D3DXVECTOR3& pos);
		const D3DXVECTOR4& getPosition() const;

		void setDirection(const D3DXVECTOR3& dir);
		const D3DXVECTOR4& getDirection() const;

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