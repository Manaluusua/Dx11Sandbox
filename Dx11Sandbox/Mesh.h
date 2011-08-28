#ifndef DX11SANDBOX_MESH_H
#define DX11SANDBOX_MESH_H
#include <D3D11.h>
#include "ILDescriptions.h"
struct ID3D11Buffer;
struct ID3D11Device;

namespace Dx11Sandbox
{
    class Mesh
    {

        struct IndexBuffer
        {
            DXGI_FORMAT format;
            UINT indexCount;
            ID3D11Buffer *buffer;
        };

        struct VertexBuffer
        {
            UINT stride;
            UINT numVertices;
            ID3D11Buffer *buffer;
        };


    public:
        Mesh();
        virtual ~Mesh();

        virtual VertexBuffer& getVertexBuffer(){return m_vertices;}
        virtual IndexBuffer& getIndexBuffer(){return m_indices;}
        virtual void setVertexBuffer(VertexBuffer vertices){m_vertices = vertices;}
        virtual void setIndexBuffer(IndexBuffer indices){m_indices = indices;}
        
        virtual void setPrimType(D3D11_PRIMITIVE_TOPOLOGY type){m_primType = type;}
        virtual D3D11_PRIMITIVE_TOPOLOGY getPrimType(){return m_primType;}



        virtual void createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, UINT numVertices, UINT numIndices,
            DXGI_FORMAT indexFormat,MeshInputLayouts::MESH_LAYOUT_TYPE type);

        virtual bool commitMeshDataToDevice(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices, void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0,
            bool createSOBuffer = false);

    private:
         IndexBuffer m_indices;
         VertexBuffer m_vertices;
         D3D11_PRIMITIVE_TOPOLOGY m_primType;
    };

   
}
#endif