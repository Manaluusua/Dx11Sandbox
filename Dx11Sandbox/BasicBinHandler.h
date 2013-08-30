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

        void render(std::vector<CullInfo*>& objects, RenderContext* state,  Camera* camera);
    };
}

#endif