#ifndef DX11SANDBOX_GPUBUFFER_H
#define DX11SANDBOX_GPUBUFFER_H


#include "RCObject.h"
#include <D3D11.h>
namespace Dx11Sandbox
{

    class GPUBuffer :
        public RCObject
    {
    public:
		GPUBuffer();
		GPUBuffer(ID3D11Buffer* buffer);

        virtual void copyDataFromBuffer( ID3D11DeviceContext* context, GPUBuffer* buffer, UINT offsetInBytes = 0 );
        virtual void setDataFromCPUBuffer( ID3D11DeviceContext* context, void* buffer, UINT size );
        virtual void destroyGPUBuffer();

		bool allocateBuffer(ID3D11Device* device, void* data, unsigned int sizeInBytes, UINT bindFlags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT cpuAccess = 0
			, UINT miscFlags = 0, UINT structByteStride = 0);

        virtual ~GPUBuffer();
		UINT getByteCount();
        ID3D11Buffer* getBuffer();

    protected:
        
		DISABLE_COPY(GPUBuffer);
		


		UINT m_byteCount;
        ID3D11Buffer* m_buffer;
		
    };


    inline ID3D11Buffer* GPUBuffer::getBuffer()
    {
        return m_buffer;
    }


	inline UINT GPUBuffer::getByteCount()
	{
		return m_byteCount;
	}

}

#endif


