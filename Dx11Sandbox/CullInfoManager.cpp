#include "CullInfoManager.h"


namespace Dx11Sandbox
{

	CullInfoManager::CullInfoManager(void)
		:m_data( 200 )
	{
	}


	CullInfoManager::~CullInfoManager(void)
	{
	}

	CullInfoPool& CullInfoManager::getCullInfoPool(size_t index)
	{
		return m_data.getDynamicPoolVector(index);
	}
    size_t CullInfoManager::getNumberOfPools() const
	{
		return m_data.getNumberOfDynamicPoolVectors();
	}

    CullInfo** CullInfoManager::allocate()
	{
		return m_data.allocateDynamic();
	}
    void CullInfoManager::deallocate(CullInfo** obj)
	{
		m_data.deallocateDynamic(obj);
	}
    void CullInfoManager::deallocateAll()
	{
		m_data.deallocateDynamicAll();
	}

}
