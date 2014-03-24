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
		virtual void passedCulling(RenderPrimitiveGatherer *gatherer);

		void setVisible(bool value);

		void destroy();

		void setLightId(UINT id);
		UINT getLightId();

	protected:
		//calculate new bounds when light parameters change
		virtual void lightParametersChanged();
		void calculateBoundsForLight(D3DXVECTOR4& bounds) const;
		

		CullableLight(CullableLightManager* mngr);
		virtual ~CullableLight(void);

		D3DXVECTOR4 m_bounds;
		CullData** m_cullingInformation;
		CullableLightManager* m_mngr;
		UINT m_lightId;
	};

};
#endif
