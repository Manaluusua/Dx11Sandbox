#include "VertexBuffer.h"


namespace Dx11Sandbox
{


    VertexBuffer::VertexBuffer(void)
        :m_stride( 0 ),
        m_vertexCount( 0 )
    {
    }

    VertexBuffer::VertexBuffer(ID3D11Device* device, void* vertices, UINT stride,  UINT numVertices, bool makeShadowBuffer,D3D11_USAGE usage, UINT cpuAccess ,
            bool createSOBuffer)
            :
            m_stride( 0 ),
            m_vertexCount( 0 )
           
    {
		if(makeShadowBuffer){
			setShadowBuffer(vertices, numVertices * stride);
		}
        allocate( device, vertices, stride, numVertices, usage, cpuAccess, createSOBuffer );
    }

    VertexBuffer::VertexBuffer( UINT stride, UINT vertexCount, ID3D11Buffer* buffer )
        :ShadowCPUBuffer( buffer ),
        m_stride( stride ),
        m_vertexCount( vertexCount )
    {
    }

    bool VertexBuffer::allocate(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices,D3D11_USAGE usage, UINT cpuAccess ,
            bool createSOBuffer)
    {
		m_stride = 0;
		m_vertexCount = 0;

		UINT binding = createSOBuffer ? D3D11_BIND_STREAM_OUTPUT : D3D11_BIND_VERTEX_BUFFER; 
		bool res = allocateBuffer(device, vertices, stride*numVertices, binding, usage, cpuAccess);
		if (res){
			m_vertexCount = numVertices;
			m_stride = stride;
			return true;
		}
		return false;

       
    }


    
    VertexBuffer::~VertexBuffer(void)
    {

       
    }

}