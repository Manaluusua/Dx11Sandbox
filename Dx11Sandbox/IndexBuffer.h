#ifndef DX11SANDBOX_INDEXBUFFER_H
#define DX11SANDBOX_INDEXBUFFER_H


#include "GPUBuffer.h"
#include <D3D11.h>
struct ID3D11Buffer;
namespace Dx11Sandbox
{
    class IndexBuffer :
        public GPUBuffer
    {
    public:
        IndexBuffer(void);
        IndexBuffer( ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0);
        IndexBuffer( DXGI_FORMAT format, UINT indexCount, ID3D11Buffer* buffer );
        virtual ~IndexBuffer(void);

        bool allocate(ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,UINT numIndices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0);

        DXGI_FORMAT getFormat() const;

        UINT getIndexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( IndexBuffer )

        DXGI_FORMAT m_format;
        UINT m_indexCount;
    };


    

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