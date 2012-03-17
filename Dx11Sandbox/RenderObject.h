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
    class RenderObject
    {

        struct Transform
        {
            Transform():translation(0,0,0),scale(1,1,1),rotation(0,0,0){}
            D3DXVECTOR3 translation;
            D3DXVECTOR3 scale;
            D3DXVECTOR3 rotation;
        };


    public:
        RenderObject();
        ~RenderObject();

        Transform transform;
        D3DXVECTOR4 boundingSphere;
        
        Mesh* mesh;
        Material* mat;

        UINT32 renderObjectMask;
        RenderQueueFlag renderQueueFlag;
        
    };
}
#endif