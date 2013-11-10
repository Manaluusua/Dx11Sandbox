#ifndef DX11SANDBOX_RenderBinHandler_H
#define DX11SANDBOX_RenderBinHandler_H

#include "RCObject.h"
#include <vector>


namespace Dx11Sandbox
{
    class Camera;
    class CullableGeometry;
	class RenderData;
    class RenderContext;

    class GeometryBinHandler: public RCObject
    {
    public:
		virtual ~GeometryBinHandler(void){};

        virtual  RenderData** setupForRendering(RenderData** objects, unsigned int objectCount, unsigned int *objectsOutCount, RenderContext* state) = 0;
        

    };
}

#endif