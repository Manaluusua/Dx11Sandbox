#include "RenderCamera.h"
#include "RenderBin.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "DXUT.h"
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

		state->getImmediateContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );

		std::map<RenderQueueID, std::vector<RenderData*> >& renderObjects = renderBin.getGeometryBins();

		unsigned int count = 0;
		auto mapIter = renderObjects.begin();
		RenderQueueID queue = Dx11Sandbox::RENDERQUEUE_FIRST;

		m_renderer->renderBegin(this, &renderBin.getLights(), state);

		//before CullableLight pass
		while(mapIter != renderObjects.end() && queue < Dx11Sandbox::RENDERQUEUE_AFTERLIGHTPASS){
			std::vector<RenderData*>& objects = mapIter->second;

			if(objects.size() > 0)
			{
				count = 0;
				queue = mapIter->first;
				RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
				RenderData** renderData = handler->setupForRendering(objects.data(),objects.size(), &count, state);
				if (renderData == 0 || count == 0) continue;
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
				count = 0;
				queue = mapIter->first;
				RCObjectPtr<GeometryBinHandler> handler = renderBin.getGeometryHandlerForBin(queue);
				RenderData** renderData = handler->setupForRendering(objects.data(),objects.size(), &count, state);
				if (renderData == 0 || count == 0) continue;
				m_renderer->render(renderData, count);
			}

			
			++mapIter;
		}

		m_renderer->renderEnd();

		if(!m_renderListeners.empty())
		{
			std::for_each(m_renderListeners.begin(), m_renderListeners.end(), [this,&renderBin, state](RenderCameraListener* listener) { listener->cameraEndedRendering(*this,renderBin,state);  });
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

	void RenderCamera::setRenderMask(RenderLayer mask){
		m_renderMask = mask;
	}

	RenderLayer RenderCamera::getRenderMask() const
	{
			return m_renderMask;
	}

	void RenderCamera::startedCulling()
	{
		if(!m_renderListeners.empty())
		{
			std::for_each(m_renderListeners.begin(), m_renderListeners.end(), [this](RenderCameraListener* listener) { listener->cameraPreCull(*this);  });
		}
	}

};
