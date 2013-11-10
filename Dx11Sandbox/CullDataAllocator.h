#ifndef DX11SANDBOX_CULLINFOMANAGER_H
#define DX11SANDBOX_CULLINFOMANAGER_H

#include "DynamicPoolAllocator.h"
#include "CullData.h"

namespace Dx11Sandbox
{

	typedef PoolVector<AllocationUnit<CullData> > CullDataPool;
	typedef DynamicPoolAllocator<CullData> CullDataAllocator;
};

#endif
