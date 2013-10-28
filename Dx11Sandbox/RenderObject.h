#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H

#include "RenderData.h"
#include "CullInfo.h"
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
	class CullInfo;
	class Mesh;
	class Material;
	class RenderObjectManager;

	class RenderObject : public RenderData
	{
		friend class RenderObjectManager;

	public:
		void setBoundingSphere(const D3DXVECTOR4& bounds); 
        void setRenderMask(RenderMask mask);
        void setRenderQueue(RenderQueueID queue);

		void setVisible(bool value);

		RenderQueueID getRenderQueue() const;

		void destroy();

	protected:
		RenderObject();
		virtual ~RenderObject();

		DISABLE_COPY(RenderObject);

		CullInfo** m_cullingInformation;
		RenderObjectManager* m_mngr;

		RenderQueueID m_renderQueue;
	};


	inline RenderQueueID RenderObject::getRenderQueue() const
	{
		return m_renderQueue;
	}
	
};

#endif