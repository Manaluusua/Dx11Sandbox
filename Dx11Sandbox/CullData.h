#ifndef DX11SANDBOX_CULLINFO_H
#define DX11SANDBOX_CULLINFO_H
#include "CommonTypes.h"
#include "CommonMath.h"


namespace Dx11Sandbox
{

	class Cullable;

    class CullData
    {
    public:
        
        CullData();
        ~CullData();

		Vec4 boundingSphere;
        Cullable* object;
    };
}
#endif