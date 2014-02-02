#ifndef DX11SANDBOX_LIGHT_H
#define DX11SANDBOX_LIGHT_H
#include "CommonTypes.h"
#include <d3dx9math.h>

namespace Dx11Sandbox
{
	class Light
	{
	public:

		enum LightType
		{
			OMNI,
			DIRECTIONAL,
			SPOT
		};

		Light(void);
		Light(LightType type, const D3DXVECTOR4& color,const D3DXVECTOR4& params );
		virtual ~Light(void);

		

		void setColor( const D3DXVECTOR4& color );
		const D3DXVECTOR4& getColor() const;

		virtual void setLightParameters(const D3DXVECTOR4& params); 
		const D3DXVECTOR4& getLightParameters() const;

		LightType getLightType() const;
		virtual void setLightType(LightType type);

	protected:
		LightType m_lightType;
		D3DXVECTOR4 m_color;
		D3DXVECTOR4 m_params;
	};

};

#endif