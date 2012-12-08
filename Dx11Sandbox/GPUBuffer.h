#ifndef DX11SANDBOX_GPUBUFFER_H
#define DX11SANDBOX_GPUBUFFER_H


#include "RCObject.h"

struct ID3D11Buffer;
struct ID3D11DeviceContext;

namespace Dx11Sandbox
{
    class GPUBuffer :
        public RCObject
    {
    public:
        

        virtual void copyDataFromBuffer( ID3D11DeviceContext* context, GPUBuffer* buffer, UINT offsetInBytes = 0 );
        virtual void setDataFromCPUBuffer( ID3D11DeviceContext* context, void* buffer, UINT size );
        virtual void destroyGPUBuffer();

        virtual ~GPUBuffer();

        ID3D11Buffer* getBuffer();

    protected:
        GPUBuffer();
        GPUBuffer( ID3D11Buffer* buffer );
        //TO DO: copy
        DISABLE_COPY( GPUBuffer )
        ID3D11Buffer* m_buffer;

        //void setDataUsingUpdateResource( ID3D11DeviceContext* context, void* data, int dataSize,  int offset );
        //void setDataUsingMapResource( ID3D11DeviceContext* context, void* data, int dataSize,  int offset );
    };


    inline ID3D11Buffer* GPUBuffer::getBuffer()
    {
        return m_buffer;
    }

}

#endif


