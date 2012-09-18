#ifndef DX11SANDBOX_MESH_H
#define DX11SANDBOX_MESH_H
#include <D3D11.h>
#include "ILDescriptions.h"
#include <vector>
#include "CommonUtilities.h"
#include "RCObjectPtr.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ID3D11Buffer;
struct ID3D11Device;



namespace Dx11Sandbox
{
    class VertexBuffer;
    class IndexBuffer;
    class RenderContext;
    class Mesh
    {



    public:
        Mesh();
        virtual ~Mesh();

        VertexBuffer* getVertexBuffer(){return m_vertices;}
        IndexBuffer* getIndexBuffer(){return m_indices;}
        void setVertexBuffer(VertexBuffer *vertices){m_vertices = vertices;}
        void setIndexBuffer(IndexBuffer *indices){m_indices = indices;}
        
        void setPrimType(D3D11_PRIMITIVE_TOPOLOGY type){m_primType = type;}
        D3D11_PRIMITIVE_TOPOLOGY getPrimType(){return m_primType;}

        void createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, UINT numVertices, UINT numIndices,
            DXGI_FORMAT indexFormat,MeshInputLayouts::MESH_LAYOUT_TYPE type);

        void createVertexBuffer(ID3D11Device* device,BYTE** vbuffers, UINT numVertices, 
            MeshInputLayouts::MESH_LAYOUT_TYPE type);

         void createIndexBuffer(ID3D11Device* device, BYTE* ibuffer, UINT numIndices,
            DXGI_FORMAT indexFormat);



        bool commitIndexDataToDevice(ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0,
            bool createSOBuffer = false);

        bool commitVertexDataToDevice(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0,
            bool createSOBuffer = false);

        bool bind(RenderContext *context);

    private:

        DISABLE_COPY(Mesh)
        
        RCObjectPtr<IndexBuffer> m_indices;
        RCObjectPtr<VertexBuffer> m_vertices;
        D3D11_PRIMITIVE_TOPOLOGY m_primType;
        
    };

   
}
#endif