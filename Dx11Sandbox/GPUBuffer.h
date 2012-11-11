#ifndef DX11SANDBOX_GPUBUFFER_H
#define DX11SANDBOX_GPUBUFFER_H


#include "RCObject.h"

class ID3D11Buffer;
class ID3D11DeviceContext;

namespace Dx11Sandbox
{
    class GPUBuffer :
        public RCObject
    {
    public:
        GPUBuffer();
        GPUBuffer( ID3D11Buffer* buffer );

        //void setDataToBuffer( ID3D11DeviceContext* context, void* data, int dataSize,  int offset = 0 );

        void setDataFromBuffer( ID3D11DeviceContext* context, ID3D11Buffer* fromBuffer, int offset );

        virtual ~GPUBuffer();

        ID3D11Buffer* getBuffer();

    protected:

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


