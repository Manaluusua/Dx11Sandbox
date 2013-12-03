#ifndef DX11SANDBOX_GEOMETRY_H
#define DX11SANDBOX_GEOMETRY_H

#include "RenderData.h"
#include "CommonUtilities.h"
namespace Dx11Sandbox
{

	class Geometry : public RenderData
	{
	public:
		Geometry(void);
		virtual ~Geometry(void);

		virtual void setRenderMask(RenderLayer mask);
        virtual void setRenderQueue(RenderQueueID queue);

		RenderQueueID getRenderQueue() const;
		RenderLayer getRenderMask() const;
	protected:
		RenderLayer m_renderLayer;
		RenderQueueID m_renderQueue;

	};

	inline RenderQueueID Geometry::getRenderQueue() const
	{
		return m_renderQueue;
	}

	inline RenderLayer Geometry::getRenderMask() const
	{
		return m_renderLayer;
	}
};
#endif