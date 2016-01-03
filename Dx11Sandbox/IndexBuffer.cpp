#include "IndexBuffer.h"


namespace Dx11Sandbox
{


    IndexBuffer::IndexBuffer(void)
        :m_format( DXGI_FORMAT_UNKNOWN ),
        m_indexCount( 0 )
    {
    }

    IndexBuffer::IndexBuffer( ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,uint32_t numIndices,bool makeShadowBuffer, D3D11_USAGE usage, uint32_t cpuAccess)
            :m_format( DXGI_FORMAT_UNKNOWN ),
        m_indexCount( 0 )
    {
		if(makeShadowBuffer){
			setShadowBuffer(indices, (indexFormat == DXGI_FORMAT_R16_UINT?2:4)*numIndices);
		}
        allocate(device, indices, indexFormat, numIndices, usage, cpuAccess );
    }

    IndexBuffer::IndexBuffer( DXGI_FORMAT format, uint32_t indexCount, ID3D11Buffer* buffer )
        :ShadowCPUBuffer( buffer ),
        m_format( format ),
        m_indexCount( indexCount )
    {

    }

    bool IndexBuffer::allocate(ID3D11Device* device,void* indices,
            DXGI_FORMAT indexFormat,uint32_t numIndices,D3D11_USAGE usage , uint32_t cpuAccess )
    {


        m_format = DXGI_FORMAT_UNKNOWN;
        m_indexCount = 0;
		bool result = allocateBuffer(device, indices, (indexFormat == DXGI_FORMAT_R16_UINT ? 2 : 4)*numIndices, D3D11_BIND_INDEX_BUFFER, usage, cpuAccess);
		if (result){
			m_format = indexFormat;
			m_indexCount = numIndices;
			return true;
		}
		
		return false;
    }

    IndexBuffer::~IndexBuffer(void)
    {

       
    }


}