#ifndef DX11SANDBOX_RENDERER_H
#define DX11SANDBOX_RENDERER_H

#include <vector>
#include <BaseTsd.h>


namespace Dx11Sandbox
{
    class Camera;
    class RenderObject;
    class RenderContext;

    class Renderer
    {
    public:
        Renderer(void);
        ~Renderer(void);

        UINT32 rendererMask;

        virtual void render(std::vector<const RenderObject*>& objects, RenderContext* state, Camera* camera) = 0;
        

    };
}

#endif