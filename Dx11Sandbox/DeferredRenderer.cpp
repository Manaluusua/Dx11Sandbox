#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "BasicForwardRenderer.h"

namespace Dx11Sandbox
{

	DeferredRenderer::DeferredRenderer()
	{
	}


	DeferredRenderer::~DeferredRenderer()
	{
	}

	void DeferredRenderer::renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state)
	{
		m_state = state;
		m_cam = cam;
		m_lights = lights;
		m_forwardRenderer->renderBegin(cam, lights, state);
		m_deferredPathActive = m_lights && m_lights->size() > 0u;
	}
	void DeferredRenderer::render(RenderData** objects, unsigned int objectCount)
	{
		if (!m_deferredPathActive){
			m_forwardRenderer->render(objects, objectCount);
			return;
		}
	}
	void DeferredRenderer::renderEnd()
	{
		m_forwardRenderer->renderEnd();
	}

	void DeferredRenderer::renderingQueue(RenderQueueID id)
	{
		m_forwardRenderer->renderingQueue(id);
	}
}