#ifndef DX11SANDBOX_MESH_H
#define DX11SANDBOX_MESH_H
#include <D3D11.h>
#include <vector>
#include "CommonUtilities.h"
#include "InputLayoutDescription.h"
#include "RCObject.h"
#include "RCObjectPtr.h"


struct ID3D11Buffer;
struct ID3D11Device;



namespace Dx11Sandbox
{
	class InputLayoutDescription;
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

        void createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, uint32_t numVertices, uint32_t numIndices,
            DXGI_FORMAT indexFormat,const InputLayoutDescription& inputLayout);

        void createVertexBuffer(ID3D11Device* device,BYTE** vbuffers, uint32_t numVertices, 
			const InputLayoutDescription& inputLayout);

        void createIndexBuffer(ID3D11Device* device, BYTE* ibuffer, uint32_t numIndices,
            DXGI_FORMAT indexFormat);


		void setIndicesToDrawCount(int count);
		void setIndicesToDrawOffest(uint32_t offset);

		uint32_t getIndicesToDrawCount() const;
		uint32_t getIndicesToDrawOffset() const;

		const InputLayoutDescription& getInputLayout() const;

        bool bind(RenderContext *context);

    private:

		DISABLE_COPY(Mesh)


		InputLayoutDescription m_inputLayout;
        RCObjectPtr<IndexBuffer> m_indices;
        RCObjectPtr<VertexBuffer> m_vertices;
        D3D11_PRIMITIVE_TOPOLOGY m_primType;
		int m_indicesToDraw;
		int m_indexOffset;
    };


   

   
}
#endif