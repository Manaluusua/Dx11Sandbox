#ifndef DX11SANDBOX_DEFERREDRENDERER_H
#define DX11SANDBOX_DEFERREDRENDERER_H

#include "Renderer.h"


namespace Dx11Sandbox
{
	class GBuffer;
	class BasicForwardRenderer;
	class DeferredRenderer: public Renderer
	{
	public:
		DeferredRenderer();
		virtual ~DeferredRenderer();

		virtual void renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state);
		virtual void render(RenderData** objects, unsigned int objectCount);
		virtual void renderEnd();
		virtual void renderingQueue(RenderQueueID id);

	private:
		BasicForwardRenderer* m_forwardRenderer;
		RenderContext* m_state;
		std::vector<Light*>* m_lights;
		Camera* m_cam;
		GBuffer* m_gbuffer;
		bool m_deferredPathActive = false;
	};

}
#endif