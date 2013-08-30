#ifndef DX11SANDBOX_RenderBinHandler_H
#define DX11SANDBOX_RenderBinHandler_H

#include "RCObject.h"
#include <vector>
#include <BaseTsd.h>


namespace Dx11Sandbox
{
    class Camera;
    class CullInfo;
    class RenderContext;

    class RenderBinHandler: public RCObject
    {
    public:
        RenderBinHandler(void);
        ~RenderBinHandler(void);

        virtual void render(std::vector<CullInfo*>& objects, RenderContext* state, Camera* camera) = 0;
        

    };
}

#endif