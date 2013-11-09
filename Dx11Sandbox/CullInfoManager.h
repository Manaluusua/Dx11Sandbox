#ifndef DX11SANDBOX_CULLINFOMANAGER_H
#define DX11SANDBOX_CULLINFOMANAGER_H

#include "DynamicPoolAllocator.h"
#include "CullInfo.h"

namespace Dx11Sandbox
{

	typedef PoolVector<AllocationUnit<CullInfo> > CullInfoPool;
	typedef DynamicPoolAllocator<CullInfo> CullInfoManager;
};

#endif
