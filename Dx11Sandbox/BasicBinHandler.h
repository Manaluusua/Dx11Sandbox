#ifndef DX11SANDBOX_BasicBinHandler_H
#define DX11SANDBOX_BasicBinHandler_H

#include "GeometryBinHandler.h"
namespace Dx11Sandbox
{
    class BasicBinHandler: public GeometryBinHandler
    {
    public:
        BasicBinHandler(void);
        ~BasicBinHandler(void);

        RenderData** setupForRendering(RenderData** objects, unsigned int objectCount,  unsigned int *objectsOutCount, RenderContext* state);
    };
}

#endif