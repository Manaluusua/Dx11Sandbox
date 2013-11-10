#ifndef DX11SANDBOX_CULLABLEGEOMETRY_H
#define DX11SANDBOX_CULLABLEGEOMETRY_H

#include "RenderData.h"
#include "Cullable.h"
#include "CullData.h"

#include "CommonUtilities.h"


namespace Dx11Sandbox
{
	template<class T>
	class CullableObjectManager;
	class CullableGeometry;
	class CullData;
	class Mesh;
	class Material;

	typedef CullableObjectManager<CullableGeometry> CullableGeometryManager;

	class CullableGeometry : public RenderData, public Cullable
	{
		
		friend class CullableObjectManager<CullableGeometry>;

	public:

		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        void setRenderMask(RenderLayer mask);
        void setRenderQueue(RenderQueueID queue);

		void setVisible(bool value);

		virtual CullableType GetCullableType() const;

		RenderQueueID getRenderQueue() const;

		void destroy();

	protected:

		CullableGeometry(CullableGeometryManager* mngr);
		virtual ~CullableGeometry();

		void acquireCullData();
		void returnCullData();
		void updateCullData();

		DISABLE_COPY(CullableGeometry);

		D3DXVECTOR4 m_bounds;
		CullData** m_cullingInformation;
		CullableGeometryManager* m_mngr;
		RenderLayer m_renderLayer;
		RenderQueueID m_renderQueue;
		
	};


	inline RenderQueueID CullableGeometry::getRenderQueue() const
	{
		return m_renderQueue;
	}
	
};

#endif