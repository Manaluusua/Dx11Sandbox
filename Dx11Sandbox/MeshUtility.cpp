#include "MeshUtility.h"
#include "DXUT.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "Texture.h"
#include "PixelBox.h"
#include "Material.h"
#include <cmath>
#include "RenderObject.h"
#include "MeshManager.h"
#include "SceneManager.h"
namespace Dx11Sandbox
{

    MeshUtility::MeshUtility(void)
    {
    }


    MeshUtility::~MeshUtility(void)
    {
    }


    Mesh* MeshUtility::createSkyBoxMesh(ID3D11Device *device, const string& name)
    {
        Mesh *mesh = MeshManager::getSingleton()->createMesh(name);
        if(!mesh)
            return 0;

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


    RenderObject* MeshUtility::createTerrainFromHeightMap(ID3D11Device *device, SceneManager* mngr, const  wstring& heightmapName,Material* mat, float scaleX, float scaleZ,float scaleY, unsigned int pagesX, unsigned int pagesZ, unsigned int tesselationFactor)
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

        //allocate renderObjects
        string terrainName("terrain");
        terrainName = terrainName + numberToString(generateID());
        RenderObject* objects;
        objects = mngr->allocateStatic(pagesZ*pagesX);

        Mesh* vertices = MeshManager::getSingleton()->createMesh(terrainName + "Vertices");
        
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
                positions[i*totalPointsX + j] = D3DXVECTOR3(x,y,z);
            }
        }

        //normals & UVS

         for(i=0,zz=0;i<totalPointsZ;++i,zz+=segmentZ)
        {
            for(j=0,xx=0;j<totalPointsX;++j, xx+=segmentX)
            {
                
                normals[i*totalPointsX + j] = D3DXVECTOR3(0,1,0);
                UV[i*totalPointsX*2 + j*2] = 0.5*((float)(j))/tesselationFactor;
                UV[i*totalPointsX*2 + j*2 + 1] = 0.5*((float)(i))/tesselationFactor;
            }
        }
        
        vertices->createVertexBuffer(device,ptr,totalPointsX*totalPointsZ,MeshInputLayouts::POS3NORM3TEX2);

        terrainName = terrainName + "Incides";
        Mesh* mesh;
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
                
                Mesh* mesh = MeshManager::getSingleton()->createMesh(terrainName + numberToString(pz*pagesX + px));
                mesh->setSharedVertices(true);
                mesh->createIndexBuffer(device,(BYTE*)indices,(tesselationFactor)*(tesselationFactor)*6,DXGI_FORMAT_R32_UINT);
                mesh->setVertexBuffer(vertices->getVertexBuffer());
                objects[pz*pagesX + px].mesh = mesh;
                objects[pz*pagesX + px].mat = mat;

            }
        }

        delete[] positions;
        delete[] normals;
        delete[] UV;
        delete indices;
        delete pixb;


        //
        
        TextureManager::getSingleton()->releaseTexture(heightmapName);

        return 0;
    }


    float MeshUtility::getHeightForPosition(const PixelBox * const map, float x, float y, float scale)
    {
        float heights[4];
        float temp,frac1 = map->getWidth()*x, frac2 = map->getHeight()*y;

        UINT x1 = std::floor(frac1);
        UINT y1 = std::floor(frac2);
        UINT x2 = std::ceil(frac1);
        UINT y2 = std::ceil(frac2);

        x2 = min(x2, map->getWidth()-1);
        y2 = min(y2, map->getHeight()-1);

        frac1 = std::modf(frac1,&temp); 
        frac2 = std::modf(frac2,&temp);

        frac1 = frac1*0.5 + (1.f-std::cos(frac1*PI))*0.25f;
        frac2 = frac2*0.5 + (1.f-std::cos(frac2*PI))*0.25f;

        heights[0] = (((float)(map->getPixel(x1,y1).r))/256)*scale;
        heights[1] = (((float)(map->getPixel(x2,y1).r))/256)*scale;
        heights[2] = (((float)(map->getPixel(x1,y2).r))/256)*scale;
        heights[3] = (((float)(map->getPixel(x2,y2).r))/256)*scale;

        return ((1-frac1)*heights[0] + frac1*heights[1])*(1-frac2) + ((1-frac1)*heights[2] + frac1*heights[3])*(frac2);
    }
}
