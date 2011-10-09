#ifndef DX11SANDBOX_BASICRENDERER_H
#define DX11SANDBOX_BASICRENDERER_H

#include "Renderer.h"
namespace Dx11Sandbox
{
    class BasicRenderer: public Renderer
    {
    public:
        BasicRenderer(void);
        ~BasicRenderer(void);

    void render(RenderObject* object, RenderContext* state, CBaseCamera* camera);
    };
}

#endif