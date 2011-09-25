#include "MeshUtility.h"
#include "DXUT.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "Texture.h"
#include "PixelBox.h"
#include "Material.h"
#include <cmath>
#include "CompositeObject.h"
#include "BasicRenderObject.h"

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
        D3DXVECTOR3 positions[8];
        D3DXVECTOR3 UV[8];
        UINT16 indices[36];
        BYTE* ptr[2];

        //float turn = std::sqrt(2.f);

        ptr[0] = (BYTE*)positions;
        ptr[1] = (BYTE*)UV;

        positions[0].x = -1.0f;UV[0].x = -1.f;
        positions[0].y = -1.0f;UV[0].y = -1.f;
        positions[0].z = 1.0f;UV[0].z = 1.f;
        
        positions[1].x = 1.0f;UV[1].x = 1.0f;
        positions[1].y = -1.0f;UV[1].y = -1.0f;
        positions[1].z = 1.0f;UV[1].z = 1.0f;
        
        positions[2].x = 1.0f;UV[2].x = 1.0f;
        positions[2].y = -1.0f;UV[2].y = -1.0f;
        positions[2].z = -1.0f;UV[2].z = -1.0f;
        
        positions[3].x = -1.0f;UV[3].x = -1.0f;
        positions[3].y = -1.0f;UV[3].y = -1.0f;
        positions[3].z = -1.0f;UV[3].z = -1.0f;

        positions[4].x = -1.0f;UV[4].x = -1.0f;
        positions[4].y = 1.0f;UV[4].y = 1.0f;
        positions[4].z = 1.0f;UV[4].z = 1.0f;

        positions[5].x = 1.0f;UV[5].x = 1.0f;
        positions[5].y = 1.0f;UV[5].y = 1.0f;
        positions[5].z = 1.0f;UV[5].z = 1.0f;

        positions[6].x = 1.0f;UV[6].x = 1.0f;
        positions[6].y = 1.0f;UV[6].y = 1.0f;
        positions[6].z = -1.0f;UV[6].z = -1.0f;

        positions[7].x = -1.0f;UV[7].x = -1.0f;
        positions[7].y = 1.0f;UV[7].y = 1.0f;
        positions[7].z = -1.0f;UV[7].z = -1.0f;


        indices[0] = 2;
        indices[1] = 0;
        indices[2] = 1;
        indices[3] = 3;
        indices[4] = 0;
        indices[5] = 2;

        indices[6] = 7;
        indices[7] = 0;
        indices[8] = 3;
        indices[9] = 4;
        indices[10] = 0;
        indices[11] = 7;

        indices[12] = 5;
        indices[13] = 1;
        indices[14] = 0;
        indices[15] = 0;
        indices[16] = 4;
        indices[17] = 5;

        indices[18] = 2;
        indices[19] = 1;
        indices[20] = 5;
        indices[21] = 5;
        indices[22] = 6;
        indices[23] = 2;

        indices[24] = 3;
        indices[25] = 2;
        indices[26] = 6;
        indices[27] = 6;
        indices[28] = 7;
        indices[29] = 3;

        indices[30] = 7;
        indices[31] = 6;
        indices[32] = 4;
        indices[33] = 4;
        indices[34] = 6;
        indices[35] = 5;

        mesh->createMeshFromBuffers(device, ptr, (BYTE*)indices, 8,36,DXGI_FORMAT_R16_UINT,MeshInputLayouts::POS3TEX3);
        return mesh;
    }


    CompositeObject* MeshUtility::createTerrainFromHeightMap(ID3D11Device *device,const  wstring& heightmapName,Material* mat, float scaleX, float scaleZ,float scaleY, unsigned int pagesX, unsigned int pagesZ, unsigned int tesselationFactor)
    {
        TextureManager::getSingleton()->createTexture(device, heightmapName, heightmapName, D3D11_CPU_ACCESS_READ, D3D11_USAGE_STAGING);
        Texture *tex = TextureManager::getSingleton()->getTexture(heightmapName);

        
        float pageSizeX = scaleX/pagesX;
        float pageSizeZ = scaleZ/pagesZ;

        float segmentX = pageSizeX/tesselationFactor;
        float segmentZ = pageSizeZ/tesselationFactor;

        float offsetX =-scaleX*0.5f;
        float offsetZ =-scaleZ*0.5f;

        unsigned int totalPointsX = pagesX*tesselationFactor;
        unsigned int totalPointsZ = pagesZ*tesselationFactor;


        if(!tex)
        {
            showErrorDialog("Couldn't load heightmap texture");
            return 0;
        }

        PixelBox* pixb = tex->readPixelBoxFromTexture();
        
        if(!pixb)
        {
            showErrorDialog("Unable to blit the texture. Format/usage incorrect?");
            return 0;
        }

        CompositeObject* composite = new CompositeObject();
        composite->setMaterial(mat);
        Mesh* vertices = new Mesh;
        Mesh* pages = new Mesh[pagesZ*pagesX];
        composite->setVertexBuffers(vertices);
        composite->setIndexBuffers(pages,pagesZ*pagesX);
        

        //
        float x, y, z;
        float xx,zz;
        unsigned int i,j;
        D3DXVECTOR3 *positions = new D3DXVECTOR3[totalPointsX*totalPointsZ];
        D3DXVECTOR3 *normals = new D3DXVECTOR3[totalPointsX*totalPointsZ];
        float* UV = new float[totalPointsX*totalPointsZ*2];
        BYTE* ptr[3];

        ptr[0] = (BYTE*)positions;
        ptr[1] = (BYTE*)normals;
        ptr[2] = (BYTE*)UV;
        //create "point cloud"
        //positions

        for(i=0,zz=0;i<totalPointsZ;++i,zz+=segmentZ)
        {
            for(j=0,xx=0;j<totalPointsX;++j, xx+=segmentX)
            {
                x = xx + offsetX;
                z = zz + offsetZ;
                y=getHeightForPosition(pixb, xx/scaleX,zz/scaleZ, scaleY);
                positions[i*totalPointsZ + j] = D3DXVECTOR3(x,y,z);
            }
        }

        //normals & UVS

         for(i=0,zz=0;i<totalPointsZ;++i,zz+=segmentZ)
        {
            for(j=0,xx=0;j<totalPointsX;++j, xx+=segmentX)
            {
                
                normals[i*totalPointsZ + j] = D3DXVECTOR3(0,1,0);
                UV[i*totalPointsZ*2 + j*2] = 3*((float)(j))/tesselationFactor;
                UV[i*totalPointsZ*2 + j*2 + 1] = 3*((float)(i))/tesselationFactor;
            }
        }
        
        vertices->createMeshFromBuffers(device, ptr, 0,totalPointsX*totalPointsZ,0,DXGI_FORMAT_R32_UINT,MeshInputLayouts::POS3NORM3TEX2);

        UINT32 *indices = new UINT32[(tesselationFactor)*(tesselationFactor)*6];
        unsigned int pwidth, pheight;
        //separate to pages and triangulate
        for(unsigned int pz = 0;pz < pagesZ;++pz)
        {
            if(pz == (pagesZ-1))
            {
                pheight = tesselationFactor - 1;
            }else
            {
                pheight = tesselationFactor;
            }
            for(unsigned int px = 0;px < pagesX; ++px)
            {
                if(px == (pagesX-1))
                {
                    pwidth = tesselationFactor-1;
                }else
                {
                    pwidth = tesselationFactor;
                }
                for(unsigned int i=0; i< pheight; ++i)
                {
                    for(unsigned int j=0; j< pwidth; ++j)
                    {
                        indices[(i*(pwidth) + j)*6] = (UINT32)j+px*tesselationFactor+i*totalPointsX + pz*totalPointsX*tesselationFactor;
                        indices[(i*(pwidth) + j)*6 +1] = (UINT32)j+px*tesselationFactor+(i+1)*totalPointsX+ pz*totalPointsX*tesselationFactor;
                        indices[(i*(pwidth) + j)*6 +2] = (UINT32)j+px*tesselationFactor+1+i*totalPointsX+ pz*totalPointsX*tesselationFactor;

                        indices[(i*(pwidth) + j)*6 +3] = (UINT32)j+px*tesselationFactor+1+i*totalPointsX+ pz*totalPointsX*tesselationFactor;
                        indices[(i*(pwidth) + j)*6 +4] = (UINT32)j+px*tesselationFactor+(i+1)*totalPointsX+ pz*totalPointsX*tesselationFactor;
                        indices[(i*(pwidth) + j)*6 +5] = (UINT32)j+px*tesselationFactor+1+(i+1)*totalPointsX+ pz*totalPointsX*tesselationFactor;
                    }
                }

                pages[pz*pagesX + px].createMeshFromBuffers(device,0,(BYTE*)indices,0,(tesselationFactor)*(tesselationFactor)*6,DXGI_FORMAT_R32_UINT,MeshInputLayouts::POS3NORM3TEX2);

            }
        }

        delete[] positions;
        delete[] normals;
        delete[] UV;
        delete indices;
        delete pixb;


        //
        
        TextureManager::getSingleton()->releaseTexture(heightmapName);

        return composite;
    }


    float MeshUtility::getHeightForPosition(const PixelBox * const map, float x, float y, float scale)
    {
        UINT x1 = std::floor(map->getWidth()*x);
        UINT y1 = std::floor(map->getHeight()*y);

        return (((float)(map->getPixel(x1,y1).r))/256)*scale;
    }
}
