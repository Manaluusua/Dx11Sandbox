#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include "RenderData.h"
#include "CullInfo.h"
#include "Cullable.h"
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
	class CullInfo;
	class Mesh;
	class Material;
	class RenderObjectManager;

	class CullableGeometry : public RenderData, public Cullable
	{
		friend class RenderObjectManager;

	public:
		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        void setRenderMask(RenderMask mask);
        void setRenderQueue(RenderQueueID queue);

		void setVisible(bool value);

		virtual CullableType GetCullableType() const;

		RenderQueueID getRenderQueue() const;

		void destroy();

	protected:
		CullableGeometry(RenderObjectManager* mngr);
		virtual ~CullableGeometry();

		DISABLE_COPY(CullableGeometry);

		CullInfo** m_cullingInformation;
		RenderObjectManager* m_mngr;

		RenderQueueID m_renderQueue;
	};


	inline RenderQueueID CullableGeometry::getRenderQueue() const
	{
		return m_renderQueue;
	}
	
};

#endif