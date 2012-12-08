#ifndef DX11SANDBOX_SHADOWCPUBUFFER_H
#define DX11SANDBOX_SHADOWCPUBUFFER_H

#include "GpuBuffer.h"

namespace Dx11Sandbox 
{

    class ShadowCPUBuffer :
        public GPUBuffer
    {
    public:
        ShadowCPUBuffer(void);
        ShadowCPUBuffer( ID3D11Buffer* buffer );
        virtual ~ShadowCPUBuffer(void);

        void setShadowBuffer( void* buffer, unsigned int size );
        void* getShadowBuffer();
        unsigned int getShadowBufferSize();

        void destroyShadowBuffer();

    private:
        void* m_cpuBuffer;
        unsigned int  m_cpuBufferSize;
    };
}
#endif