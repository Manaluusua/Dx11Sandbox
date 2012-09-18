#include "IndexBuffer.h"


namespace Dx11Sandbox
{


    IndexBuffer::IndexBuffer(void)
        :m_format( DXGI_FORMAT_UNKNOWN ),
        m_indexCount( 0 ),
        m_buffer( 0 )
    {
    }

    IndexBuffer::IndexBuffer( DXGI_FORMAT format, UINT indexCount, ID3D11Buffer* buffer )
        :m_format( format ),
        m_indexCount( indexCount ),
        m_buffer( buffer )
    {

    }

    IndexBuffer::~IndexBuffer(void)
    {

        SAFE_RELEASE( m_buffer );
    }


}