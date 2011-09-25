#ifndef DX11SANDBOX_MESHUTILITY_H
#define DX11SANDBOX_MESHUTILITY_H
#include "CommonUtilities.h"

struct ID3D11Device;

namespace Dx11Sandbox
{
    class Mesh;
    class CompositeObject;
    class Material;
    class PixelBox;
    class MeshUtility
    {
    public:
        MeshUtility(void);
        ~MeshUtility(void);

        static Mesh* createSkyBoxMesh(ID3D11Device *device);

        static CompositeObject* createTerrainFromHeightMap(ID3D11Device *device,const  wstring& heightmapName,Material* mat, float scaleX, float scaleZ,float scaleY, unsigned int pagesX=10, unsigned int pagesZ=10, unsigned int tesselationFactor=100.f);
    private:

        inline static float getHeightForPosition(const PixelBox * const map, float x, float y, float scale);
    };
}

#endif