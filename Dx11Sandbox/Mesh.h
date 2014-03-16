#ifndef DX11SANDBOX_MESH_H
#define DX11SANDBOX_MESH_H
#include <D3D11.h>
#include "ILDescriptions.h"
#include <vector>
#include "CommonUtilities.h"
#include "RCObject.h"
#include "RCObjectPtr.h"


struct ID3D11Buffer;
struct ID3D11Device;



namespace Dx11Sandbox
{
    class VertexBuffer;
    class IndexBuffer;
    class RenderContext;
    class Mesh: public RCObject
    {



    public:
        Mesh();
        virtual ~Mesh();

        VertexBuffer* getVertexBuffer();
        IndexBuffer* getIndexBuffer();
        void setVertexBuffer(VertexBuffer *vertices);
        void setIndexBuffer(IndexBuffer *indices);
        
		void setPrimType(D3D11_PRIMITIVE_TOPOLOGY type);
		D3D11_PRIMITIVE_TOPOLOGY getPrimType();

        void createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, UINT numVertices, UINT numIndices,
            DXGI_FORMAT indexFormat,MeshInputLayouts::MESH_LAYOUT_TYPE type);

        void createVertexBuffer(ID3D11Device* device,BYTE** vbuffers, UINT numVertices, 
            MeshInputLayouts::MESH_LAYOUT_TYPE type);

        void createIndexBuffer(ID3D11Device* device, BYTE* ibuffer, UINT numIndices,
            DXGI_FORMAT indexFormat);


		void setIndicesToDrawCount(int count);
		void setIndicesToDrawOffest(UINT offset);

		UINT getIndicesToDrawCount() const;
		UINT getIndicesToDrawOffset() const;

        bool bind(RenderContext *context);

    private:

        DISABLE_COPY(Mesh)
        
        RCObjectPtr<IndexBuffer> m_indices;
        RCObjectPtr<VertexBuffer> m_vertices;
        D3D11_PRIMITIVE_TOPOLOGY m_primType;
		int m_indicesToDraw;
		int m_indexOffset;
    };


   

   
}
#endif