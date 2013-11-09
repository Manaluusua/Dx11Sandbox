#include "RenderCamera.h"
#include "RenderBin.h"
#include "RenderContext.h"
#include "Renderer.h"
#include <algorithm>

namespace Dx11Sandbox
{

	RenderCamera::RenderCamera(void)
		:m_cameraPriority(0),
		m_renderMask(0xFFFFFFFF)
	{

	}
	 
	RenderCamera::~RenderCamera(void)
	{

	}

	void RenderCamera::render( RenderBin& renderBin, RenderContext* state)
	{
		if(m_renderer.rawPtr() == 0) return;

		if(!m_renderListeners.empty())
		{
			std::for_each(m_renderListeners.begin(), m_renderListeners.end(), [this,&renderBin, state](RenderCameraListener* listener) { listener->cameraStartedRendering(*this,renderBin,state);  });
		}

		std::map<RenderQueueID, std::vector<RenderData*> >& renderObjects = renderBin.getGeometryBins();

		RenderData** renderData = 0; 
		unsigned int count = 0;
		auto mapIter = renderObjects.begin();
		RenderQueueID queue = Dx11Sandbox::RENDERQUEUE_FIRST;

		//before light pass
		while(mapIter != renderObjects.end() && queue < Dx11Sandbox::RENDERQUEUE_AFTERLIGHTPASS){
			queue = mapIter->first;
			std::vector<RenderData*>& objects = mapIter->second;
			RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
			handler->setupForRendering(objects.data(),objects.size(), renderData, &count, state);
			m_renderer->render(renderData, count, state);
		}


		//lightpass

		//rest
		while(mapIter != renderObjects.end()){
			queue = mapIter->first;
			std::vector<RenderData*>& objects = mapIter->second;
			RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
			handler->setupForRendering(objects.data(),objects.size(), renderData, &count, state);
			m_renderer->render(renderData, count, state);
		}


	}

	void RenderCamera::addRenderListener(RenderCameraListener *l)
	{
		if(l == 0) return;
		m_renderListeners.insert(l);
	}

	void RenderCamera::removeRenderListener(RenderCameraListener *l)
	{
		if(l == 0) return;
		m_renderListeners.erase(l);
	}


	void RenderCamera::setRenderer(RCObjectPtr<Renderer> renderer)
	{
		m_renderer = renderer;
	}

	RCObjectPtr<Renderer> RenderCamera::getRenderer() const
	{
		return m_renderer;
	}

	void RenderCamera::setCameraPriority(INT32 priority)
	{
		m_cameraPriority = priority;
	}

	INT32 RenderCamera::getCameraPriority() const
	{
		return m_cameraPriority;
	}

	void RenderCamera::setRenderMask(RenderMask mask){
		m_renderMask = mask;
	}

	RenderMask RenderCamera::getRenderMask() const
	{
			return m_renderMask;
	}

};
