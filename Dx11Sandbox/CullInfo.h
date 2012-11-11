#ifndef DX11SANDBOX_RENDEROBJECT_H
#define DX11SANDBOX_RENDEROBJECT_H
#include <BaseTsd.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{

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
        int binIDFlag;
        
    };
}
#endif