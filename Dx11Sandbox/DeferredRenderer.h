#ifndef DX11SANDBOX_DEFERREDRENDERER_H
#define DX11SANDBOX_DEFERREDRENDERER_H

#include "Renderer.h"
#include "RCObjectPtr.h"

namespace Dx11Sandbox
{
	class GBuffer;
	class BasicForwardRenderer;
	class DeferredRenderer: public Renderer
	{
	public:
		DeferredRenderer(RenderContext* state);
		virtual ~DeferredRenderer();

		virtual void renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state);
		virtual void render(RenderData** objects, unsigned int objectCount);
		virtual void renderEnd();
		virtual void renderingQueue(RenderQueueID id);

	private:
		void bindGBuffer();
		void unbindGBuffer();
		void renderToGBuffer(RenderData** objects, unsigned int objectCount);
		void doLightPass();
		


		RCObjectPtr<BasicForwardRenderer> m_forwardRenderer;
		RenderContext* m_state;
		std::vector<Light*>* m_lights;
		Camera* m_cam;
		GBuffer* m_gbuffer;
		bool m_gbufferBound;
		bool m_deferredPathActive;
	};

}
#endif