#ifndef DX11SANDBOX_RenderBinHandler_H
#define DX11SANDBOX_RenderBinHandler_H

#include "RCObject.h"
#include <vector>


namespace Dx11Sandbox
{
    class Camera;
    class RenderObject;
	class RenderData;
    class RenderContext;

    class RenderBinHandler: public RCObject
    {
    public:
		virtual ~RenderBinHandler(void){};

        virtual void setupForRendering(RenderObject* objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state) = 0;
        

    };
}

#endif