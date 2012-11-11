#ifndef DX11SANDBOX_RENDERER_H
#define DX11SANDBOX_RENDERER_H

#include "RCObject.h"
#include <vector>
#include <BaseTsd.h>


namespace Dx11Sandbox
{
    class Camera;
    class CullInfo;
    class RenderContext;

    class Renderer: public RCObject
    {
    public:
        Renderer(void);
        ~Renderer(void);

        UINT32 rendererMask;

        virtual void render(std::vector<CullInfo*> objects, RenderContext* state, Camera* camera) = 0;
        

    };
}

#endif