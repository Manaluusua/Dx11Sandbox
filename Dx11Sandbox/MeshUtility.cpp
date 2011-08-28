#include "MeshUtility.h"
#include "DXUT.h"
#include "Mesh.h"

namespace Dx11Sandbox
{

    MeshUtility::MeshUtility(void)
    {
    }


    MeshUtility::~MeshUtility(void)
    {
    }


    Mesh* MeshUtility::createSkyBoxMesh(ID3D11Device *device)
    {
        Mesh *mesh = new Mesh();
        D3DXVECTOR3 positions[4];
        D3DXVECTOR3 normals[4];
        D3DXVECTOR2 UV[4];
        UINT16 indices[6];
        BYTE* ptr[3];

        ptr[0] = (BYTE*)positions;
        ptr[1] = (BYTE*)normals;
        ptr[2] = (BYTE*)UV;

        positions[0].x = -1.0f;normals[0].x = 0.0f;
        positions[0].y = 0.0f;normals[0].y = 1.0f;
        positions[0].z = 1.0f;normals[0].z = 0.0f;
        
        positions[1].x = 1.0f;normals[1].x = 0.0f;
        positions[1].y = 0.0f;normals[1].y = 1.0f;
        positions[1].z = 1.0f;normals[1].z = 0.0f;
        
        positions[2].x = 1.0f;normals[2].x = 0.0f;
        positions[2].y = 0.0f;normals[2].y = 1.0f;
        positions[2].z = -1.0f;normals[2].z = 0.0f;
        
        positions[3].x = -1.0f;normals[3].x = 0.0f;
        positions[3].y = 0.0f;normals[3].y = 1.0f;
        positions[3].z = -1.0f;normals[3].z = 0.0f;

        UV[0].x = 0.0f;
        UV[0].y = 0.0f;
        UV[1].x = 1.0f;
        UV[1].y = 0.0f;
        UV[2].x = 1.0f;
        UV[2].y = 1.0f;
        UV[3].x = 0.0f;
        UV[3].y = 1.0f;

        indices[0] = 1;
        indices[1] = 0;
        indices[2] = 2;
        indices[3] = 2;
        indices[4] = 0;
        indices[5] = 3;

        mesh->createMeshFromBuffers(device, ptr, (BYTE*)indices, 4,6,DXGI_FORMAT_R16_UINT,MeshInputLayouts::POS3NORM3TEX2);
        return mesh;
    }

}
