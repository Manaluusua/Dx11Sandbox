#ifndef DX11SANDBOX_CULLINFOMANAGER_H
#define DX11SANDBOX_CULLINFOMANAGER_H

#include "DynamicPoolAllocator.h"
#include "Singleton.h"
#include "CommonUtilities.h"
#include "CullInfo.h"

namespace Dx11Sandbox
{

	typedef PoolVector<AllocationUnit<CullInfo> > CullInfoPool;
	class CullInfoManager : public Singleton<CullInfoManager>
	{
		SINGLETON(CullInfoManager)
	public:
		
		
        virtual ~CullInfoManager(void);

		CullInfoPool& getCullInfoPool(size_t index);
        size_t getNumberOfPools() const;

        CullInfo** allocate();
        void deallocate(CullInfo** obj);
        void deallocateAll();

    private:
		CullInfoManager(void);
        DISABLE_COPY(CullInfoManager);

		DynamicPoolAllocator<CullInfo> m_data;
        
    };
	

};

#endif
