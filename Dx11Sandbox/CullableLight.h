#ifndef DX11SANDBOX_CULLABLELIGHT_H
#define DX11SANDBOX_CULLABLELIGHT_H
#include "CommonTypes.h"
#include "Cullable.h"
#include "Light.h"

namespace Dx11Sandbox
{
	template<class T>
	class CullableObjectManager;
	class CullableLight;
	class CullData;

	typedef CullableObjectManager<CullableLight> CullableLightManager;

	class CullableLight: public Light, public Cullable
	{
		friend class CullableObjectManager<CullableLight>;
	public:
		
		virtual void setLightType(LightType type);
		virtual void setLightParameters(const D3DXVECTOR4& params); 

		virtual void passedCulling(RenderPrimitiveGatherer *gatherer);

		void setVisible(bool value);

		void destroy();

	protected:

		CullableLight(CullableLightManager* mngr);
		virtual ~CullableLight(void);

		D3DXVECTOR4 m_bounds;
		CullData** m_cullingInformation;
		CullableLightManager* m_mngr;
	};

};
#endif
