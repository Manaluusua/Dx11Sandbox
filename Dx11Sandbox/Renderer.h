#ifndef DX11SANDBOX_RENDERER_H
#define DX11SANDBOX_RENDERER_H

#include "CommonUtilities.h"
#include "RCObject.h"

namespace Dx11Sandbox
{
	class RenderData;
	class RenderContext;
	class Renderer : public RCObject
	{
	public:
		
		virtual void render(RenderData** objects, unsigned int objectCount, RenderContext* state) = 0;

	protected:
		virtual ~Renderer(void){}
	};

};
#endif