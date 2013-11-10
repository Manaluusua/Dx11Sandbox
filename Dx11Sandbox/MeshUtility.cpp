#include "MeshUtility.h"
#include "MathUtil.h"

#include "Mesh.h"
#include "TextureManager.h"
#include "Texture.h"
#include "PixelBox.h"
#include "Material.h"
#include "CullableGeometry.h"
#include "MeshManager.h"
#include "SceneManager.h"
#include "IndexBuffer.h"
#include "DXUT.h"
#include <cmath>
namespace Dx11Sandbox
{

    //inlines
    D3DXVECTOR3 getNormalForPosition(int indexX, int indexY, int maxX, int maxY, D3DXVECTOR3 *positions)
    {
        int startX = max(indexX-1,0);
        int endX = min(indexX+1,maxX-1);
        int startY = max(indexY-1,0);
        int endY = min(indexY+1,maxY-1);

        D3DXVECTOR3 vec1 = positions[indexY * maxX +  startX] -  positions[indexY * maxX +  endX];
        D3DXVECTOR3 vec2 = positions[startY * maxX +  indexX] -  positions[endY * maxX +  indexX];
        D3DXVECTOR3 normal;
        D3DXVec3Cross(&normal,&vec2, &vec1);
        D3DXVec3Normalize(&normal, &normal);

        return normal;
    }

    
    float getHeightForPosition(const PixelBox * const map, float x, float y, float scale)
    {
        float heights[4];
        float temp,frac1 = map->getWidth()*x, frac2 = map->getHeight()*y;

        UINT x1 = static_cast<UINT>( std::floor(frac1) );
        UINT y1 = static_cast<UINT>( std::floor(frac2) );
        UINT x2 = static_cast<UINT>( std::ceil(frac1) );
        UINT y2 = static_cast<UINT>( std::ceil(frac2) );

        x2 = min(x2, map->getWidth()-1);
        y2 = min(y2, map->getHeight()-1);

        frac1 = std::modf(frac1,&temp); 
        frac2 = std::modf(frac2,&temp);

        frac1 = frac1*0.5f + (1.f-std::cos(frac1*(float)MathUtil::PI))*0.25f;
        frac2 = frac2*0.5f + (1.f-std::cos(frac2*(float)MathUtil::PI))*0.25f;

        heights[0] = (((float)(map->getPixel(x1,y1).r))/256)*scale;
        heights[1] = (((float)(map->getPixel(x2,y1).r))/256)*scale;
        heights[2] = (((float)(map->getPixel(x1,y2).r))/256)*scale;
        heights[3] = (((float)(map->getPixel(x2,y2).r))/256)*scale;

        return ((1-frac1)*heights[0] + frac1*heights[1])*(1-frac2) + ((1-frac1)*heights[2] + frac1*heights[3])*(frac2);
    }
    

    //MeshUtility
    MeshUtility::MeshUtility(void)
    {
    }


    MeshUtility::~MeshUtility(void)
    {
    }


    Mesh* MeshUtility::createSkyBoxMesh(ID3D11Device *device, const string& name)
    {
        Mesh *mesh = MeshManager::singleton()->createMesh(name);
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


    CullableGeometry* MeshUtility::createFinitePlane(ID3D11Device *device,SceneManager* mngr, const string& name, D3DXVECTOR3 normal, float d, float extends1, float extends2, int tesselationFactorX, int tesselationFactorZ)
    {

        assert( tesselationFactorX > 0 && tesselationFactorZ > 0 );

        float incrementX = (1.f/(tesselationFactorX))*2;
        float incrementZ = (1.f/(tesselationFactorZ))*2;

        unsigned int pointsX = tesselationFactorX+1;
        unsigned int pointsZ = tesselationFactorZ+1;

        extends1 *= 0.5f;
        extends2 *= 0.5f;

        D3DXVec3Normalize(&normal, &normal);
        D3DXVECTOR3 vec1;
        D3DXVECTOR3 vec2;
        MathUtil::calculateOrthogonalVector(normal, vec1);
        D3DXVec3Cross(&vec2,&normal, &vec1);

        D3DXVec3Normalize(&vec1,&vec1);
        D3DXVec3Normalize(&vec2,&vec2);

		CullableGeometry* ro = mngr->createCullableGeometry();
        Mesh* mesh = MeshManager::singleton()->createMesh(name + "Mesh");
        
        //
        float x=0, y=0, z=0;
        unsigned int i,j;
        float xx,zz;

        D3DXVECTOR3 *positions = new D3DXVECTOR3[pointsX*pointsZ];
        D3DXVECTOR3 *normals = new D3DXVECTOR3[pointsX*pointsZ];
        float* UV = new float[pointsX*pointsZ*2];
        BYTE* ptr[3];

        ptr[0] = (BYTE*)positions;
        ptr[1] = (BYTE*)normals;
        ptr[2] = (BYTE*)UV;
        //create "point cloud"
        //positions
        for(i=0u,zz=-1.f;i<pointsZ;++i,zz+=incrementZ)
        {
            for(j=0u,xx = -1.f;j<pointsX;++j, xx+=incrementX)
            {
                
                positions[i*pointsX + j] =normal*-d + vec1*extends1*xx + vec2*extends2*zz;
            }
        }

        //normals & UVS
        for(i=0u;i<pointsZ;++i)
        {
            for(j=0u;j<pointsX;++j)
            {
                
                normals[i*pointsX + j] = getNormalForPosition(j,i,pointsX, pointsZ,positions);
                UV[i*pointsX*2 + j*2] = ((float)(j))/tesselationFactorX;
                UV[i*pointsX*2 + j*2 + 1] = ((float)(i))/tesselationFactorZ;
            }
        }
        
        mesh->createVertexBuffer(device,ptr,pointsX*pointsZ,MeshInputLayouts::POS3NORM3TEX2);

        UINT indicesCount = (tesselationFactorX)*(tesselationFactorZ)*6;
        UINT32 *indices = new UINT32[indicesCount];

        int indicesX = pointsX-1;
        int indicesZ = pointsZ-1;

        for(int i=0; i< indicesZ; ++i)
        {
            for(int j=0; j< indicesX; ++j)
            {
                indices[(i*(indicesX) + j)*6] = (UINT32)j+i*pointsX;
                indices[(i*(indicesX) + j)*6 +1] = (UINT32)j+1+i*pointsX;
                indices[(i*(indicesX) + j)*6 +2] = (UINT32)j+(i+1)*pointsX;

                indices[(i*(indicesX) + j)*6 +3] = (UINT32)j+(i+1)*pointsX;
                indices[(i*(indicesX) + j)*6 +4] = (UINT32)j+1+i*pointsX;
                indices[(i*(indicesX) + j)*6 +5] = (UINT32)j+1+(i+1)*pointsX;

            }

        }

        mesh->createIndexBuffer(device,(BYTE*)indices,indicesCount,DXGI_FORMAT_R32_UINT);

		ro->setMesh( mesh );
		ro->setBoundingSphere( calculateBoundingSphereForPositions(indices,indicesCount , positions) );

        delete[] positions;
        delete[] normals;
        delete[] UV;
        delete[] indices;



        //
        

        return ro;
        /*Mesh *mesh = MeshManager::getSingleton()->createMesh(name);
        if(!mesh || tesselationFactorX < 1 || tesselationFactorY < 1)
            return 0;

        int numOfVertices = (1 + tesselationFactorX) * (1+tesselationFactorY);


        D3DXVECTOR3 *pos = new D3DXVECTOR3[numOfVertices];
        D3DXVECTOR3 *norm = new D3DXVECTOR3[numOfVertices];
        D3DXVECTOR2 *UV = new D3DXVECTOR2[numOfVertices];

        BYTE* ptr[3];

        ptr[0] = (BYTE*)pos;
        ptr[1] = (BYTE*)norm;
        ptr[2] = (BYTE*)UV;


        //normalize just in case
        D3DXVec3Normalize(&normal, &normal);
        D3DXVECTOR3 vec1;
        D3DXVECTOR3 vec2;
        MathUtil::calculateOrthogonalVector(normal, vec1);
        D3DXVec3Cross(&vec2,&normal, &vec1);

        D3DXVec3Normalize(&vec1,&vec1);
        D3DXVec3Normalize(&vec2,&vec2);

        pos[0] = normal*-d - vec1*extends1 - vec2*extends2;
        pos[1] = normal*-d + vec1*extends1 - vec2*extends2;
        pos[2] = normal*-d - vec1*extends1 + vec2*extends2;
        pos[3] = normal*-d + vec1*extends1 + vec2*extends2;

        UV[0] = D3DXVECTOR2(0,0);
        UV[1] = D3DXVECTOR2(1,0);
        UV[2] = D3DXVECTOR2(0,1);
        UV[3] = D3DXVECTOR2(1,1);

        for(int i=0;i<4;++i)
        {
            norm[i] = normal;
        }



        mesh->createMeshFromBuffers(device, ptr, 0, 4,0,DXGI_FORMAT_R16_UINT,MeshInputLayouts::POS3NORM3TEX2);
        mesh->setPrimType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        SAFE_DELETE_ARRAY(pos);
        SAFE_DELETE_ARRAY(norm);
        SAFE_DELETE(UV);
        return mesh;*/
    }


    void MeshUtility::createTerrainFromHeightMap(ID3D11Device *device, SceneManager* mngr, const  wstring& heightmapName,Material* mat, float scaleX, float scaleZ,float scaleY, unsigned int pagesX, unsigned int pagesZ, unsigned int tesselationFactor)
    {
        TextureManager::singleton()->createTexture(device, heightmapName, heightmapName, D3D11_CPU_ACCESS_READ, D3D11_USAGE_STAGING);
        Texture *tex = TextureManager::singleton()->getTexture(heightmapName);

        
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
            return;
        }

        PixelBox* pixb = tex->readPixelBoxFromTexture();
        
        if(!pixb)
        {
            showErrorDialog("Unable to blit the texture. Format/usage incorrect?");
            return;
        }

        //allocate renderObjects
        string terrainName("terrain");
        terrainName = terrainName + numberToString(generateID());


        
        string tempMeshName = terrainName + "Vertices";
        Mesh* vertices = MeshManager::singleton()->createMesh(tempMeshName);
        
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
                
                normals[i*totalPointsX + j] = getNormalForPosition(j,i,totalPointsX, totalPointsZ,positions);
                UV[i*totalPointsX*2 + j*2] = 0.5f*((float)(j))/tesselationFactor;
                UV[i*totalPointsX*2 + j*2 + 1] = 0.5f*((float)(i))/tesselationFactor;
            }
        }
        
        vertices->createVertexBuffer(device,ptr,totalPointsX*totalPointsZ,MeshInputLayouts::POS3NORM3TEX2);

        terrainName = terrainName + "Indices";

        UINT indicesCount = (tesselationFactor)*(tesselationFactor)*6;
        UINT32 *indices = new UINT32[indicesCount];
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
                
                //create object
				CullableGeometry* obj = mngr->createCullableGeometry();

                Mesh* mesh = MeshManager::singleton()->createMesh(terrainName + numberToString(pz*pagesX + px));

                //mesh->createIndexBuffer(device,(BYTE*)indices,indicesCount,DXGI_FORMAT_R32_UINT);
                //just create empty, non-allocated index buffer
                mesh->setIndexBuffer( new IndexBuffer( DXGI_FORMAT_R32_UINT, indicesCount, 0 ) );
                //create shadow buffer
                mesh->getIndexBuffer()->setShadowBuffer( indices, 4 * indicesCount );

                mesh->setVertexBuffer(vertices->getVertexBuffer());
				obj->setMesh( mesh );
				obj->setMaterial( mat );
				obj->setBoundingSphere( calculateBoundingSphereForPositions(indices,pwidth * pheight * 6 , positions) );
				obj->setRenderMask(Dx11Sandbox::RENDERLAYER_DEFAULT_OPAQUE);
				obj->setRenderQueue(Dx11Sandbox::RENDERQUEUE_TERRAIN);

            }
        }

        delete[] positions;
        delete[] normals;
        delete[] UV;
        delete[] indices;
        delete pixb;
        

        //
        

        MeshManager::singleton()->destroyMesh( tempMeshName );
        TextureManager::singleton()->releaseTexture(heightmapName);

    }


    D3DXVECTOR4 MeshUtility::calculateBoundingSphereForPositions(const UINT32 *indices,UINT numIndices,const D3DXVECTOR3* positions)
    {
        
        float minimum[3] = {FLT_MAX,FLT_MAX,FLT_MAX};
        float maximum[3] = {-FLT_MAX,-FLT_MAX,-FLT_MAX};
        
        //find point cloud extends
        for(unsigned int i=0;i<numIndices;++i)
        {
            UINT32 index = indices[i];
            const float *position = (float*)&positions[index];
            for(int j=0;j<3;++j)
            {
                if(position[j]<minimum[j])
                {
                    minimum[j] = position[j];
                }
                if(position[j]>maximum[j])
                {
                    maximum[j] = position[j];
                }
            }
        }

        float len = sqrt(pow(maximum[0] - minimum[0],2) + pow(maximum[1] - minimum[1],2)  + pow(maximum[2] - minimum[2],2))*0.5f ;

        D3DXVECTOR4 sphere((minimum[0] + maximum[0])*0.5f, (minimum[1] + maximum[1])*0.5f,(minimum[2] + maximum[2])*0.5f,len);

        return sphere;
    }
}

