#ifndef DX11SANDBOX_BASICGEOMETRY_H
#define DX11SANDBOX_BASICGEOMETRY_H

#include "Geometry.h"
#include "RenderData.h"
#include "CommonUtilities.h"
namespace Dx11Sandbox
{
	//Basic Geometry implementation that has non-shared renderdata
	class BasicGeometry : public Geometry
	{
	public:
		BasicGeometry(void);
		virtual ~BasicGeometry(void);

		virtual void setRenderMask(RenderLayer mask);
        virtual void setRenderQueue(RenderQueueID queue);

		virtual RenderQueueID getRenderQueue() const;
		virtual RenderLayer getRenderMask() const;
		virtual RenderData& getRenderData();
	protected:
		RenderLayer m_renderLayer;
		RenderQueueID m_renderQueue;
		RenderData m_renderData;
	};

	
};
#endif