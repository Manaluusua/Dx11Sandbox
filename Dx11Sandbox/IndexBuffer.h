#ifndef DX11SANDBOX_INDEXBUFFER_H
#define DX11SANDBOX_INDEXBUFFER_H


#include "RCObject.h"
#include <D3D11.h>
struct ID3D11Buffer;
namespace Dx11Sandbox
{
    class IndexBuffer :
        public RCObject
    {
    public:
        IndexBuffer(void);
        IndexBuffer( DXGI_FORMAT format, UINT indexCount, ID3D11Buffer* buffer );
        virtual ~IndexBuffer(void);


        void setFormat( DXGI_FORMAT format );

        void setIndexCount( UINT indexCount );

        void setBuffer( ID3D11Buffer* buffer );

        ID3D11Buffer* getBuffer();

        DXGI_FORMAT getFormat() const;

        UINT getIndexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( IndexBuffer )

        DXGI_FORMAT m_format;
        UINT m_indexCount;
        ID3D11Buffer* m_buffer;
    };


        inline void IndexBuffer::setFormat( DXGI_FORMAT format )
        {
            m_format = format;
        }

        inline void IndexBuffer::setIndexCount( UINT indexCount )
        {
            m_indexCount = indexCount;
        }

        inline void IndexBuffer::setBuffer( ID3D11Buffer* buffer )
        {
            m_buffer = buffer;
        }

        inline ID3D11Buffer* IndexBuffer::getBuffer()
        {
            return m_buffer;
        }

        inline DXGI_FORMAT IndexBuffer::getFormat() const
        {
            return m_format;
        }

        inline UINT IndexBuffer::getIndexCount() const
        {
            return m_indexCount;
        }

}

#endif