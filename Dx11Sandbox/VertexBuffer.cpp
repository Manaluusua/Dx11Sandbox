#include "VertexBuffer.h"
#include <D3D11.h>

namespace Dx11Sandbox
{


    VertexBuffer::VertexBuffer(void)
        :m_stride( 0 ),
        m_vertexCount( 0 ),
        m_buffer( 0 )
    {
    }

    VertexBuffer::VertexBuffer( UINT stride, UINT vertexCount, ID3D11Buffer* buffer )
        :m_stride( stride ),
        m_vertexCount( vertexCount ),
        m_buffer( buffer )
    {
    }

    VertexBuffer::~VertexBuffer(void)
    {

        SAFE_RELEASE( m_buffer );
    }


}