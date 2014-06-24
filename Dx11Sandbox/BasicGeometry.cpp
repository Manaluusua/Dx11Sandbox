#include "BasicGeometry.h"
namespace Dx11Sandbox
{

	BasicGeometry::BasicGeometry(void)
		:m_renderQueue( 0 ),
		m_renderLayer( RENDERLAYER_DEFAULT_OPAQUE )
	{
	}


	BasicGeometry::~BasicGeometry(void)
	{
	}

	void BasicGeometry::setRenderMask(RenderLayer mask)
	{	
		m_renderLayer = mask;
	}

	void BasicGeometry::setRenderQueue(RenderQueueID queue)
	{
		m_renderQueue = queue;
	}

	RenderQueueID BasicGeometry::getRenderQueue() const
	{
		return m_renderQueue;
	}

	RenderLayer BasicGeometry::getRenderMask() const
	{
		return m_renderLayer;
	}

	RenderData& BasicGeometry::getRenderData()
	{
		return m_renderData;
	}
}