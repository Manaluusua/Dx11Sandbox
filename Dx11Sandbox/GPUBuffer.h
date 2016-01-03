#ifndef DX11SANDBOX_GPUBUFFER_H
#define DX11SANDBOX_GPUBUFFER_H


#include "RCObject.h"
#include <stdint.h>
#include <D3D11.h>
namespace Dx11Sandbox
{

    class GPUBuffer :
        public RCObject
    {
    public:
		GPUBuffer();
		GPUBuffer(ID3D11Buffer* buffer);

        virtual void copyDataFromBuffer( ID3D11DeviceContext* context, GPUBuffer* buffer, uint32_t offsetInBytes = 0 );
        virtual void setDataFromCPUBuffer( ID3D11DeviceContext* context, void* buffer, uint32_t size );
        virtual void destroyGPUBuffer();

		bool allocateBuffer(ID3D11Device* device, void* data, unsigned int sizeInBytes, uint32_t bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t cpuAccess = 0
			, uint32_t miscFlags = 0, uint32_t structByteStride = 0);

        virtual ~GPUBuffer();
		uint32_t getByteCount();
        ID3D11Buffer* getBuffer();

    protected:
        
		DISABLE_COPY(GPUBuffer);
		


		uint32_t m_byteCount;
        ID3D11Buffer* m_buffer;
		
    };


    inline ID3D11Buffer* GPUBuffer::getBuffer()
    {
        return m_buffer;
    }


	inline uint32_t GPUBuffer::getByteCount()
	{
		return m_byteCount;
	}

}

#endif


