#include "IndexBuffer.h"


namespace Dx11Sandbox
{


    IndexBuffer::IndexBuffer(void)
        :m_format( DXGI_FORMAT_UNKNOWN ),
        m_indexCount( 0 )
    {
    }

    IndexBuffer::IndexBuffer( ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage, UINT cpuAccess)
            :m_format( DXGI_FORMAT_UNKNOWN ),
        m_indexCount( 0 )
    {
        allocate(device, indices, indexFormat, numIndices, usage, cpuAccess );
    }

    IndexBuffer::IndexBuffer( DXGI_FORMAT format, UINT indexCount, ID3D11Buffer* buffer )
        :ShadowCPUBuffer( buffer ),
        m_format( format ),
        m_indexCount( indexCount )
    {

    }

    bool IndexBuffer::allocate(ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage , UINT cpuAccess )
    {
        if(m_buffer)
        {
            SAFE_RELEASE( m_buffer );
            m_buffer = 0;
            m_format = DXGI_FORMAT_UNKNOWN;
            m_indexCount = 0;
        }

        D3D11_BUFFER_DESC indBuffDesc;
	    D3D11_SUBRESOURCE_DATA indexData;
        D3D11_SUBRESOURCE_DATA* indexDataPtr = 0;
        ID3D11Buffer* buffer;
	    HRESULT hr;

        if(numIndices>0)
        {

            

	        indBuffDesc.Usage = usage;
            indBuffDesc.ByteWidth = (indexFormat == DXGI_FORMAT_R16_UINT?2:4)*numIndices;
	        indBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	        indBuffDesc.CPUAccessFlags = cpuAccess;
	        indBuffDesc.MiscFlags = 0;
	        indBuffDesc.StructureByteStride = 0;

            if( indices )
            {
	            indexData.pSysMem = indices;
	            indexData.SysMemPitch = 0;
	            indexData.SysMemSlicePitch = 0;
                indexDataPtr = &indexData;
            }

            hr = device->CreateBuffer(&indBuffDesc, indexDataPtr, &buffer);
	        if(FAILED(hr))
	        {
		        return false;
	        }

            
            m_buffer = buffer;
            m_format = indexFormat;
            m_indexCount = numIndices;
            return true;
        }
        
	    return false;

    }

    IndexBuffer::~IndexBuffer(void)
    {

       
    }


}