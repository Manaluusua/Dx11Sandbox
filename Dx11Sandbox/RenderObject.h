#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H
#include <BaseTsd.h>
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
    class Mesh;
    class Material;
    class RenderObject
    {
    public:
        RenderObject();
        ~RenderObject();
        Mesh* mesh;
        Material* mat;
        UINT32 renderObjectMask;
        RenderQueueFlag renderQueueFlag;
        float translation[3];
        float scale[3];
        float rotation[3];
        float boundingSphere[4];
    };
}
#endif