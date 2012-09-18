#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H
#include <BaseTsd.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{


    enum RenderQueueFlag{
        RINITIAL, //first objects, background?
        RDEFAULT, //opaque
        RTRANSPARENT, //transparent
        RSCENEINPUT, //here the previous scene is available for input
        RFINAL //last
    };

    class Mesh;
    class Material;
    class CullInfo
    {



    public:
        CullInfo();
        ~CullInfo();

        D3DXVECTOR4 boundingSphere;
        Mesh* mesh;
        Material* mat;
        RenderQueueFlag renderQueueFlag;
        
    };
}
#endif