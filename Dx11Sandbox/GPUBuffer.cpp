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

  /*  void GPUBuffer::setDataToBuffer( ID3D11DeviceContext* context, void* data, int dataSize,  int offset)
    {
       if( !m_buffer )
           return;
       D3D11_BUFFER_DESC desc;
       m_buffer->GetDesc( &desc );

       if( desc.Usage == D3D11_USAGE_DEFAULT )
       {
           setDataUsingUpdateResource( context, data, dataSize, offset );

       }else if ( (desc.Usage == D3D11_USAGE_DYNAMIC) || ( desc.Usage == D3D11_USAGE_STAGING ) && (desc.CPUAccessFlags | D3D11_CPU_ACCESS_WRITE) )
       {
           setDataUsingMapResource( context, data, dataSize, offset );
       }
       
       
    }

    void GPUBuffer::setDataUsingUpdateResource( ID3D11DeviceContext* context, void* data, int dataSize,  int offset )
    {

    }

    void GPUBuffer::setDataUsingMapResource( ID3D11DeviceContext* context,void* data, int dataSize,  int offset )
    {
        D3D11_MAPPED_SUBRESOURCE resource;
        if( !FAILED( context->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource ) )  )
        {
            resource.pData = data;
        }
    }*/

    void GPUBuffer::setDataFromBuffer( ID3D11DeviceContext* context, ID3D11Buffer* fromBuffer, int offset )
    {
        if( !m_buffer )
            return;

      
    }

    GPUBuffer::~GPUBuffer()
    {
        SAFE_RELEASE( m_buffer );
    }


}