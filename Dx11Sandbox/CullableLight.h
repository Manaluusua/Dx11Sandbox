#ifndef DX11SANDBOX_CULLABLELIGHT_H
#define DX11SANDBOX_CULLABLELIGHT_H
#include "CommonTypes.h"
#include "Cullable.h"
#include "Light.h"

namespace Dx11Sandbox
{

	class CullableObjectManager;

	class CullData;

	class CullableLight: public Light, public Cullable
	{
		friend class CullableObjectManager;
	public:
		virtual void passedCulling(RenderPrimitiveGatherer *gatherer);

		void setVisible(bool value);

		void destroy();

		void setLightId(uint32_t id);
		uint32_t getLightId();

	protected:
		//calculate new bounds when light parameters change
		virtual void lightParametersChanged();
		void calculateBoundsForLight(Vec4& bounds) const;
		

		CullableLight(CullableObjectManager* mngr);
		virtual ~CullableLight(void);

		Vec4 m_bounds;
		CullData** m_cullingInformation;
		CullableObjectManager* m_mngr;
		uint32_t m_lightId;
	};

};
#endif
