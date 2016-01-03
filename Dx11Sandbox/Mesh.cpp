#include "Mesh.h"
#include "DXUT.h"
#include "RenderContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



namespace Dx11Sandbox
{
    Mesh::Mesh()
        :m_primType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
		m_indicesToDraw(-1),
		m_indexOffset(0)
    {
        
    }


    Mesh::~Mesh()
    {

    }

	void Mesh::setIndicesToDrawCount(int count)
	{
		m_indicesToDraw = count;
	}
	void Mesh::setIndicesToDrawOffest(uint32_t offset)
	{
		m_indexOffset = offset;
	}

	uint32_t Mesh::getIndicesToDrawCount() const
	{
		if (!m_indices.rawPtr()) return 0;

		return m_indicesToDraw < 0 ? m_indices->getIndexCount() : m_indicesToDraw;
	}
	uint32_t Mesh::getIndicesToDrawOffset() const
	{

		return m_indexOffset;
	}

    VertexBuffer* Mesh::getVertexBuffer()
    {
        return m_vertices.rawPtr();
    }
    
    IndexBuffer* Mesh::getIndexBuffer()
    {
        return m_indices.rawPtr();
    }

    
    void Mesh::setVertexBuffer(VertexBuffer *vertices)
    {
        m_vertices = vertices;
    }

    void Mesh::setIndexBuffer(IndexBuffer *indices)
    {
        m_indices = indices;
    }

	void Mesh::setPrimType(D3D11_PRIMITIVE_TOPOLOGY type)
	{ 
		m_primType = type; 
	}
	D3D11_PRIMITIVE_TOPOLOGY Mesh::getPrimType()
	{ 
		return m_primType; 
	}

	const InputLayoutDescription& Mesh::getInputLayout() const
	{
		return m_inputLayout;

	}

    void Mesh::createMeshFromBuffers(ID3D11Device* device,BYTE** vbuffers, BYTE* ibuffer, uint32_t numVertices, uint32_t numIndices,
		DXGI_FORMAT indexFormat, const InputLayoutDescription& inputLayout)
    {
        

        if(numVertices>0)
        {
            createVertexBuffer(device, vbuffers,numVertices,inputLayout);
        }

        if(numIndices>0)
        {
            createIndexBuffer(device,ibuffer,numIndices,indexFormat);
        }

		

  
    }


	void Mesh::createVertexBuffer(ID3D11Device* device, BYTE** vbuffers, uint32_t numVertices, const InputLayoutDescription& inputLayout)
    {
        uint32_t stride = 0;
        BYTE* vertexBuffer= 0;
		m_inputLayout = inputLayout;
        if(numVertices>0)
        {
            int numBuffers = inputLayout.getElementCount();

            assert(numBuffers>0);

           
            for(int i=0;i<numBuffers;i++)
            {
                stride += inputLayout.getElementByteWidth(i);
            }

            vertexBuffer = new BYTE[stride*numVertices];

            for(uint32_t i=0;i<numVertices;++i)
            {
                uint32_t elemOffset = 0;
                for(int j=0;j<numBuffers;++j)
                {
					uint32_t elemSize = inputLayout.getElementByteWidth(j);
                    memcpy( &vertexBuffer[i*stride + elemOffset],  &vbuffers[j][i*elemSize], elemSize);
                    elemOffset += elemSize;
                }
            }
        


            m_vertices = new VertexBuffer(device, vertexBuffer, stride, numVertices);


            delete [] vertexBuffer;
        }

    }

    void Mesh::createIndexBuffer(ID3D11Device* device, BYTE* ibuffer, uint32_t numIndices, DXGI_FORMAT indexFormat)
    {

        if(numIndices>0)
        {

            m_indices = new IndexBuffer( device, ibuffer, indexFormat, numIndices );
        }
    }



    bool Mesh::bind(RenderContext *context)
    {
        Mesh* boundMesh = context->getBoundMesh();
        if(!boundMesh || boundMesh->getVertexBuffer() != m_vertices.rawPtr())
        {
            uint32_t stride = m_vertices->getStride();
            uint32_t* strides = &stride;
            uint32_t offsets[1];
            offsets[0] = 0;
            ID3D11Buffer *buffers[1];
            buffers[0] = m_vertices->getBuffer();
            context->getImmediateContext()->IASetVertexBuffers(0,1,buffers,strides, offsets);
        }
        if(!boundMesh || boundMesh->getIndexBuffer() != m_indices.rawPtr())
        {
            if(m_indices->getIndexCount()>0)
                context->getImmediateContext()->IASetIndexBuffer(m_indices->getBuffer(),m_indices->getFormat(), 0);
        }

        if(!boundMesh || boundMesh->getPrimType() != m_primType)
        {
            context->getImmediateContext()->IASetPrimitiveTopology( m_primType );
        }
        return true;
    }
}