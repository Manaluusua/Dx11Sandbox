#ifndef DX11SANDBOX_INDEXBUFFER_H
#define DX11SANDBOX_INDEXBUFFER_H


#include "ShadowCPUBuffer.h"
struct ID3D11Buffer;
namespace Dx11Sandbox
{
    class IndexBuffer :
        public ShadowCPUBuffer
    {
    public:
        IndexBuffer(void);
        IndexBuffer( ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,uint32_t numIndices,bool makeShadowBuffer = true, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccess = 0);
        IndexBuffer( DXGI_FORMAT format, uint32_t indexCount, ID3D11Buffer* buffer );
        virtual ~IndexBuffer(void);

        bool allocate(ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,uint32_t numIndices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccess = 0);

        DXGI_FORMAT getFormat() const;

        uint32_t getIndexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( IndexBuffer )

        DXGI_FORMAT m_format;
        uint32_t m_indexCount;
    };


    

        inline DXGI_FORMAT IndexBuffer::getFormat() const
        {
            return m_format;
        }

        inline uint32_t IndexBuffer::getIndexCount() const
        {
            return m_indexCount;
        }

}

#endif