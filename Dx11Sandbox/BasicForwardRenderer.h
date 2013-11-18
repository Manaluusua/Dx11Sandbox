#ifndef DX11SANDBOX_BASICFORWARDRENDERER_H
#define DX11SANDBOX_BASICFORWARDRENDERER_H

#include "renderer.h"
#include "BasicForwardRenderer.h"
#include "BasicMaterialPropertiesSetter.h"
namespace Dx11Sandbox
{


	class BasicForwardRenderer :
		public Renderer
	{
	public:
		BasicForwardRenderer(void);
		~BasicForwardRenderer(void);
		virtual void renderBegin(RenderCamera* cam, std::vector<Light*>& lights, RenderContext* state) ;
		virtual void renderEnd();
		virtual void render(RenderData** objects, unsigned int objectCount);

	protected:
		BasicMaterialPropertiesSetter m_materialPropertySetter;
		RenderContext* m_state;
		RenderCamera* m_cam;
		
	};

}

#endif