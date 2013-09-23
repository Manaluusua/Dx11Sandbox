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
        VertexBuffer( UINT stride, UINT vertexCount, ID3D11Buffer* buffer );
        VertexBuffer( ID3D11Device* device, void* vertices, UINT stride, UINT numVertices,bool makeShadowBuffer = true, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0,
            bool createSOBuffer = false );
        virtual ~VertexBuffer(void);

        bool allocate(ID3D11Device* device, void* vertices, UINT stride, UINT numVertices,D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0,
            bool createSOBuffer = false);

        UINT getStride() const;

        UINT getVertexCount() const;

    private:

        //TO DO: copy
        DISABLE_COPY( VertexBuffer )

        UINT m_stride;
        UINT m_vertexCount;
    };


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