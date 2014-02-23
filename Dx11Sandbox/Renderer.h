#ifndef DX11SANDBOX_RENDERER_H
#define DX11SANDBOX_RENDERER_H

#include "CommonUtilities.h"
#include "RCObject.h"
#include <vector>

namespace Dx11Sandbox
{
	class RenderData;
	class RenderContext;
	class Camera;
	class Light;

	class Renderer : public RCObject
	{
	public:
		
		enum RenderTechniqueIndex {
			FORWARD_RENDERER = 0,
			DEFERRED_RENDERER = 1
		};



		virtual void renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state) = 0;
		virtual void render(RenderData** objects, unsigned int objectCount) = 0;
		virtual void renderEnd() = 0;
		virtual void renderingQueue(RenderQueueID id) = 0;
	protected:
		virtual ~Renderer(void){}
	};

};
#endif