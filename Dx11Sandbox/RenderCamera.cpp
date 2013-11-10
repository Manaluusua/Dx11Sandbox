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

		unsigned int count = 0;
		auto mapIter = renderObjects.begin();
		RenderQueueID queue = Dx11Sandbox::RENDERQUEUE_FIRST;

		m_renderer->renderBegin(this, state);

		//before light pass
		while(mapIter != renderObjects.end() && queue < Dx11Sandbox::RENDERQUEUE_AFTERLIGHTPASS){
			std::vector<RenderData*>& objects = mapIter->second;

			if(objects.size() > 0)
			{
				queue = mapIter->first;
				RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
				RenderData** renderData = handler->setupForRendering(objects.data(),objects.size(), &count, state);
				m_renderer->render(renderData, count);
			}

			++mapIter;
			
		}


		//lightpass

		//rest
		while(mapIter != renderObjects.end()){
			std::vector<RenderData*>& objects = mapIter->second;

			if(objects.size() > 0)
			{
				queue = mapIter->first;
				RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
				RenderData** renderData = handler->setupForRendering(objects.data(),objects.size(), &count, state);
				m_renderer->render(renderData, count);
			}

			
			++mapIter;
		}

		m_renderer->renderEnd();
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

	void RenderCamera::setRenderMask(RenderLayer mask){
		m_renderMask = mask;
	}

	RenderLayer RenderCamera::getRenderMask() const
	{
			return m_renderMask;
	}

};
