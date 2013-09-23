#ifndef DX11SANDBOX_CULLINFO_H
#define DX11SANDBOX_CULLINFO_H
#include "CommonTypes.h"
#include <BaseTsd.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{

	class RenderObject;

    class CullInfo
    {
    public:
        typedef int CULLINFO_FLAGS;

        CullInfo();
        ~CullInfo();

        D3DXVECTOR4 boundingSphere;
        RenderObject* object;
        int binIDFlag;
        CULLINFO_FLAGS flags;
        
    };
}
#endif