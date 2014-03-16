#include "GPUBuffer.h"
#include <D3D11.h>
namespace Dx11Sandbox
{
    GPUBuffer::GPUBuffer()
        :m_buffer( 0 )
    {

    }
    GPUBuffer::GPUBuffer( ID3D11Buffer* buffer )
    {
        m_buffer = buffer;
        
    }


    void GPUBuffer::copyDataFromBuffer( ID3D11DeviceContext* context, GPUBuffer* buffer, UINT offsetInBytes  )
    {
        if( !m_buffer || !buffer )
            return;

        D3D11_BUFFER_DESC toDesc;
        D3D11_BUFFER_DESC fromDesc;

        m_buffer->GetDesc( &toDesc );
        buffer->getBuffer()->GetDesc( &fromDesc );
#if defined( DEBUG )        
        //oops, out of boundaries
        if( toDesc.ByteWidth < (fromDesc.ByteWidth + offsetInBytes) )
        {
            showErrorDialog( "GPUBuffer::copyDataFromBuffer, tried to copy past max size of destination buffer" );
        }
#endif
        context->CopySubresourceRegion( m_buffer, 0, offsetInBytes, 0, 0, buffer->getBuffer(), 0, 0 );
      
     }

    void GPUBuffer::setDataFromCPUBuffer( ID3D11DeviceContext* context, void* buffer, UINT size )
    {
        if( !m_buffer || !buffer )
            return;

        D3D11_BUFFER_DESC toDesc;
        

        m_buffer->GetDesc( &toDesc );
  
#if defined( DEBUG )        
        //oops, out of boundaries
        if( toDesc.ByteWidth < (size + offsetInBytes) )
        {
            showErrorDialog( "GPUBuffer::copyDataFromCPUBuffer, tried to copy past max size of destination buffer" );
        }

        if( !(toDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) || toDesc.Usage != D3D11_USAGE_DYNAMIC || toDesc.Usage != D3D11_USAGE_STAGING )
        {
            showErrorDialog( "GPUBuffer::copyDataFromCPUBuffer, non-compatible usage or cpuAccessFlags at destination buffer" );
        }
#endif
        D3D11_MAPPED_SUBRESOURCE resource;
        HRESULT hResult = context->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource );

        if(hResult != S_OK)
        {
            showErrorDialog( "GPUBuffer::copyDataFromCPUBuffer, failed to map the data" );
        }

        memcpy( resource.pData, buffer, size );

        context->Unmap(m_buffer, 0);

        
    }


	bool GPUBuffer::allocateBuffer(ID3D11Device* device, void* data, unsigned int sizeInBytes, UINT bindFlags, D3D11_USAGE usage, UINT cpuAccess, UINT miscFlags, UINT structByteStride)
	{
		if (m_buffer)
		{
			SAFE_RELEASE(m_buffer);
			m_byteCount = 0;
			m_buffer = 0;
		}

		D3D11_BUFFER_DESC buffDesc;
		D3D11_SUBRESOURCE_DATA buffData;
		D3D11_SUBRESOURCE_DATA* buffDataPtr = 0;
		ID3D11Buffer* buffer;
		HRESULT hr;

		if (sizeInBytes>0)
		{
			buffDesc.Usage = usage;
			buffDesc.ByteWidth = sizeInBytes;

			buffDesc.BindFlags = bindFlags;
			

			buffDesc.CPUAccessFlags = cpuAccess;
			buffDesc.MiscFlags = miscFlags;
			buffDesc.StructureByteStride = structByteStride;

			if (data){
				buffData.pSysMem = data;
				buffData.SysMemPitch = 0;
				buffData.SysMemSlicePitch = 0;
				buffDataPtr = &buffData;
			}
			

			hr = device->CreateBuffer(&buffDesc, buffDataPtr, &buffer);
			if (FAILED(hr))
			{
				return false;
			}
			m_buffer = buffer;
			m_byteCount = sizeInBytes;
			return true;
		}
		return false;
	}

    void GPUBuffer::destroyGPUBuffer()
    {
        SAFE_RELEASE( m_buffer );
    }

    GPUBuffer::~GPUBuffer()
    {
        destroyGPUBuffer();
    }


}