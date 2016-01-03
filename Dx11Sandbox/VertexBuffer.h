#ifndef DX11SANDBOX_VERTEXBUFFER_H
#define DX11SANDBOX_VERTEXBUFFER_H


#include "ShadowCPUBuffer.h"
#include <D3D11.h>

namespace Dx11Sandbox
{
    class VertexBuffer :
        public ShadowCPUBuffer
    {
    public:
        VertexBuffer(void);
        VertexBuffer( uint32_t stride, uint32_t vertexCount, ID3D11Buffer* buffer );
        VertexBuffer( ID3D11Device* device, void* vertices, uint32_t stride, uint32_t numVertices,bool makeShadowBuffer = true, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccess = 0,
            bool createSOBuffer = false );
        virtual ~VertexBuffer(void);

        bool allocate(ID3D11Device* device, void* vertices, uint32_t stride, uint32_t numVertices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccess = 0,
            bool createSOBuffer = false);

        uint32_t getStride() const;

        uint32_t getVertexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( VertexBuffer )

        uint32_t m_stride;
        uint32_t m_vertexCount;
    };


        inline uint32_t VertexBuffer::getStride() const
        {
            return m_stride;
        }

        inline uint32_t VertexBuffer::getVertexCount() const
        {
            return m_vertexCount;
        }

}

#endif