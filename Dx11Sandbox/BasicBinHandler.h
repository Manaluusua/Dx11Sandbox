#ifndef DX11SANDBOX_BasicBinHandler_H
#define DX11SANDBOX_BasicBinHandler_H

#include "RenderBinHandler.h"
namespace Dx11Sandbox
{
    class BasicBinHandler: public RenderBinHandler
    {
    public:
        BasicBinHandler(void);
        ~BasicBinHandler(void);

        void setupForRendering(RenderObject* objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state);
    };
}

#endif