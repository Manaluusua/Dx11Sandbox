#ifndef DX11SANDBOX_MESHUTILITY_H
#define DX11SANDBOX_MESHUTILITY_H
#include "CommonUtilities.h"
#include <d3dx9math.h>

struct ID3D11Device;
namespace Dx11Sandbox
{
    class Mesh;
    class Material;
    class PixelBox;
    class SceneManager;
    class RenderObject;
    class MeshUtility
    {
    public:
        MeshUtility(void);
        ~MeshUtility(void);

        static D3DXVECTOR4 calculateBoundingSphereForPositions(const UINT32 *indices,UINT numIndices,const D3DXVECTOR3* positions);

        static Mesh* createSkyBoxMesh(ID3D11Device *device, const string& name);
        static Mesh* createFinitePlane(ID3D11Device *device, const string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2); 

        static RenderObject* createTerrainFromHeightMap(ID3D11Device *device, SceneManager* mngr, const  wstring& heightmapName,Material* mat, float scaleX, float scaleZ,float scaleY, unsigned int pagesX=10, unsigned int pagesZ=10, unsigned int tesselationFactor=100.f);
    private:

        
    };
}

#endif