#include "ShadowCPUBuffer.h"

#include <D3D11.h>
namespace Dx11Sandbox 
{


    ShadowCPUBuffer::ShadowCPUBuffer(void)
        :m_cpuBuffer(0),
        m_cpuBufferSize(0)
    {
    }

    ShadowCPUBuffer::ShadowCPUBuffer( ID3D11Buffer* buffer )
        :GPUBuffer( buffer ),
        m_cpuBuffer(0),
        m_cpuBufferSize(0)
    {

    }


    ShadowCPUBuffer::~ShadowCPUBuffer(void)
    {
        destroyShadowBuffer();
    }

    void ShadowCPUBuffer::setShadowBuffer( void* buffer, unsigned int size, bool makeCopy )
    {
        if( m_cpuBuffer )
        {
            destroyShadowBuffer();
        }

		if(makeCopy){
			m_cpuBuffer = new BYTE[size];
			memcpy( m_cpuBuffer, buffer, size );
		}else{
			m_cpuBuffer = buffer;
		}
        
        m_cpuBufferSize = size;
    } 

    void* ShadowCPUBuffer::getShadowBuffer()
    {
        return m_cpuBuffer;
    }

    unsigned int ShadowCPUBuffer::getShadowBufferSize()
    {
        return m_cpuBufferSize;
    }


    void ShadowCPUBuffer::destroyShadowBuffer()
    {
        SAFE_DELETE( m_cpuBuffer );
    }

}