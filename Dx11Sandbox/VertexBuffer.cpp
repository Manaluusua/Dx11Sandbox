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
        if( m_buffer )
        {
            SAFE_RELEASE( m_buffer );
            m_stride = 0;
            m_vertexCount = 0;
            m_buffer = 0;
        }

        D3D11_BUFFER_DESC vertBuffDesc;
	    D3D11_SUBRESOURCE_DATA vertexData;
        ID3D11Buffer* buffer;
	    HRESULT hr;

        if(numVertices>0)
        {
	        vertBuffDesc.Usage = usage;
	        vertBuffDesc.ByteWidth = stride*numVertices;
	    
            if(createSOBuffer)
            {
                vertBuffDesc.BindFlags = D3D11_BIND_STREAM_OUTPUT;
            }
            else
            {
                vertBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            }

	        vertBuffDesc.CPUAccessFlags = cpuAccess;
	        vertBuffDesc.MiscFlags = 0;
	        vertBuffDesc.StructureByteStride = 0;

	        vertexData.pSysMem = vertices;
	        vertexData.SysMemPitch = 0;
	        vertexData.SysMemSlicePitch = 0;

            hr = device->CreateBuffer(&vertBuffDesc, &vertexData, &buffer);
	        if(FAILED(hr))
	        {
		        return false;
	        }
            m_buffer = buffer;
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