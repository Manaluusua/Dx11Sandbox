#ifndef DX11SANDBOX_MESHUTILITY_H
#define DX11SANDBOX_MESHUTILITY_H
#include "CommonUtilities.h"

struct ID3D11Device;

namespace Dx11Sandbox
{
    class Mesh;
    class MeshUtility
    {
    public:
        MeshUtility(void);
        ~MeshUtility(void);

        static Mesh* createSkyBoxMesh(ID3D11Device *device);

    };
}

#endif