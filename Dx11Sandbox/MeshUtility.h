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
    class CullableGeometry;
    class MeshUtility
    {
    public:
        MeshUtility(void);
        ~MeshUtility(void);

        static D3DXVECTOR4 calculateBoundingSphereForPositions(const UINT32 *indices,UINT numIndices,const D3DXVECTOR3* positions);

        static Mesh* createSkyBoxMesh(ID3D11Device *device, const string& name);
		static Mesh* createUnitSphere(ID3D11Device *device, unsigned int tesselationZenith = 3, unsigned int tesselationAzimuth = 3, bool generateNormals = true, bool generateUvs = true, bool makeLineListInsteadOfTriangles = false);
		static Mesh* createQuad(ID3D11Device *device, const D3DXVECTOR3* corners, bool flipTextureCoordinatesX = false, bool flipTextureCoordinatesY = false);
		static Mesh* createQuad(ID3D11Device *device, float x, float y, float w, float h, bool flipTextureCoordinatesX = false, bool flipTextureCoordinatesY = false);
        static CullableGeometry* createFinitePlane(ID3D11Device *device,SceneManager* mngr, const string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2, int tesselationFactorX=1, int tesselationFactorZ=1); 

		static void createTerrainFromHeightMap(ID3D11Device *device, SceneManager* mngr, const  string& heightmapName, Material* mat, float scaleX, float scaleZ, float scaleY, unsigned int pagesX = 10, unsigned int pagesZ = 10, unsigned int tesselationFactor = 100.f);
    

        
    };
}

#endif