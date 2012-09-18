#ifndef DX11SANDBOX_VERTEXBUFFER_H
#define DX11SANDBOX_VERTEXBUFFER_H


#include "RCObject.h"

struct ID3D11Buffer;
namespace Dx11Sandbox
{
    class VertexBuffer :
        public RCObject
    {
    public:
        VertexBuffer(void);
        VertexBuffer( UINT stride, UINT vertexCount, ID3D11Buffer* buffer );
        virtual ~VertexBuffer(void);


        void setStride( UINT stride );

        void setVertexCount( UINT VertexCount );

        void setBuffer( ID3D11Buffer* buffer );

        ID3D11Buffer* getBuffer();

        UINT getStride() const;

        UINT getVertexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( VertexBuffer )

        UINT m_stride;
        UINT m_vertexCount;
        ID3D11Buffer* m_buffer;
    };


        inline void VertexBuffer::setStride( UINT stride )
        {
            m_stride = stride;
        }

        inline void VertexBuffer::setVertexCount( UINT VertexCount )
        {
            m_vertexCount = VertexCount;
        }

        inline void VertexBuffer::setBuffer( ID3D11Buffer* buffer )
        {
            m_buffer = buffer;
        }

        inline ID3D11Buffer* VertexBuffer::getBuffer()
        {
            return m_buffer;
        }

        inline UINT VertexBuffer::getStride() const
        {
            return m_stride;
        }

        inline UINT VertexBuffer::getVertexCount() const
        {
            return m_vertexCount;
        }

}

#endif