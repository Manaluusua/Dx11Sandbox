#ifndef DX11SANDBOX_DEFERREDRENDERER_H
#define DX11SANDBOX_DEFERREDRENDERER_H

#include "Renderer.h"
#include "RCObjectPtr.h"
#include "LightListToStructuredData.h"
#include "BasicMaterialPropertiesSetter.h"
namespace Dx11Sandbox
{
	class GBuffer;
	class BasicForwardRenderer;
	class Shader;
	class Texture;
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
		void initializeLightPass();
		void uninitializeLightPass();

		void copyLightPassOutputToRenderTarget();
		
		void createOutputTex(unsigned int w, unsigned int h);

		static const unsigned int s_threadsPerGroupX;
		static const unsigned int s_threadsPerGroupY;
		static const string s_copyMaterialName;

		LightListToStructuredData m_lightBuffer;
		BasicMaterialPropertiesSetter m_materialPropertySetter;
		RCObjectPtr<BasicForwardRenderer> m_forwardRenderer;
		RenderContext* m_state;
		std::vector<Light*>* m_lights;
		Camera* m_cam;
		RCObjectPtr<GBuffer> m_gbuffer;
		RCObjectPtr<Shader> m_accumulateLightsCS;
		RenderData* m_copyData;
		Texture* m_lightingOutput;
		bool m_gbufferBound;
		bool m_deferredPathActive;
	};

}
#endif