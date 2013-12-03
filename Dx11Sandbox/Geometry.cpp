#include "Geometry.h"
namespace Dx11Sandbox
{

	Geometry::Geometry(void)
		:m_renderQueue( 0 ),
		m_renderLayer( RENDERLAYER_DEFAULT_OPAQUE )
	{
	}


	Geometry::~Geometry(void)
	{
	}

	void Geometry::setRenderMask(RenderLayer mask)
	{	
		m_renderLayer = mask;
	}

	void Geometry::setRenderQueue(RenderQueueID queue)
	{
		m_renderQueue = queue;
	}
}