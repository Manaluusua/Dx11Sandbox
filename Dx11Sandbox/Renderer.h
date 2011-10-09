#ifndef DX11SANDBOX_RENDERER_H
#define DX11SANDBOX_RENDERER_H
class CBaseCamera;
namespace Dx11Sandbox
{
    class RenderObject;
    class RenderContext;

    class Renderer
    {
    public:
        Renderer(void);
        ~Renderer(void);

        virtual void render(RenderObject* object, RenderContext* state, CBaseCamera* camera) = 0;
    };
}

#endif