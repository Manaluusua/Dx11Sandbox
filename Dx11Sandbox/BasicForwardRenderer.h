#ifndef DX11SANDBOX_BASICFORWARDRENDERER_H
#define DX11SANDBOX_BASICFORWARDRENDERER_H
#include "renderer.h"

namespace Dx11Sandbox
{


	class BasicForwardRenderer :
		public Renderer
	{
	public:
		BasicForwardRenderer(void);
		~BasicForwardRenderer(void);

		virtual void render(RenderData** objects, unsigned int objectCount, RenderContext* state);
	};

}

#endif