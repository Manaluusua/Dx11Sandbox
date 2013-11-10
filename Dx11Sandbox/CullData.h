#ifndef DX11SANDBOX_CULLINFO_H
#define DX11SANDBOX_CULLINFO_H
#include "CommonTypes.h"
#include <BaseTsd.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{

	class Cullable;

    class CullData
    {
    public:
        
        CullData();
        ~CullData();

        D3DXVECTOR4 boundingSphere;
        Cullable* object;
    };
}
#endif