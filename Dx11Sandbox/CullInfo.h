#ifndef DX11SANDBOX_CULLINFO_H
#define DX11SANDBOX_CULLINFO_H
#include <BaseTsd.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{

	class RenderObject;

    class CullInfo
    {
    public:
        typedef int CULLINFO_FLAGS;
        static const CULLINFO_FLAGS CULLINFO_VISIBLE;

        CullInfo();
        ~CullInfo();

        D3DXVECTOR4 boundingSphere;
        RenderObject* object;
        int binIDFlag;
        CULLINFO_FLAGS flags;
        
    };
}
#endif