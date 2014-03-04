#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "RenderContext.h"
#include "BasicForwardRenderer.h"
#include "EnvironmentInfo.h"

namespace Dx11Sandbox
{

	DeferredRenderer::DeferredRenderer(RenderContext* state)
		:m_forwardRenderer(new BasicForwardRenderer),
		m_state(0),
		m_lights(0),
		m_cam(0),
		m_gbuffer(new GBuffer(state)),
		m_gbufferBound(false),
		m_deferredPathActive(false)
	{

	}


	DeferredRenderer::~DeferredRenderer()
	{
		SAFE_DELETE(m_gbuffer);
	}

	void DeferredRenderer::bindGBuffer()
	{
		if (m_gbufferBound) return;

		//figure out later where to get the gbuffer dimensions
		m_gbuffer->allocateBuffer(EnvironmentInfo::getScreenWidth(), EnvironmentInfo::getScreenHeight());

		m_gbuffer->setAsRenderTargets();
		m_gbufferBound = true;
	}

	void DeferredRenderer::unbindGBuffer()
	{
		if (!m_gbufferBound) return;
		m_state->popRenderTargets();
		m_gbufferBound = false;
	}

	void DeferredRenderer::renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state)
	{
		m_state = state;
		m_cam = cam;
		m_lights = lights;
		m_forwardRenderer->renderBegin(cam, lights, state);
		m_deferredPathActive = m_lights && m_lights->size() > 0u;

		if (m_deferredPathActive){
			bindGBuffer();
		}
		


	}
	void DeferredRenderer::render(RenderData** objects, unsigned int objectCount)
	{
		if (!m_deferredPathActive){
			m_forwardRenderer->render(objects, objectCount);
			return;
		}
		renderToGBuffer(objects, objectCount);
	}
	void DeferredRenderer::renderEnd()
	{
		m_forwardRenderer->renderEnd();
	}

	

	void DeferredRenderer::renderingQueue(RenderQueueID id)
	{
		m_forwardRenderer->renderingQueue(id);

		//do light pass and switch to forward
		if (id >= RENDERQUEUE_AFTERLIGHTPASS && m_deferredPathActive){
			m_deferredPathActive = false;
			doLightPass();
		}
	}


	

	void DeferredRenderer::renderToGBuffer(RenderData** objects, unsigned int objectCount)
	{

	}

	void DeferredRenderer::doLightPass()
	{
		unbindGBuffer();
	}
}