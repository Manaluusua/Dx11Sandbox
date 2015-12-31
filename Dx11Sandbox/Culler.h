#ifndef DX11SANDBOX_CULLER_H
#define DX11SANDBOX_CULLER_H

#include "RCObject.h"
#include <vector>

namespace Dx11Sandbox
{
	class CullData;
	class Cullable;
	class RenderData;
	class Frustum;
	template <typename CullData> class AllocationUnit;
	template <typename CullData> class PoolVector;

	namespace CullUtility
	{
		void cull(const Frustum& frusta, const std::vector<RenderData*>&in, std::vector<RenderData*>& out);
		void cull(const Frustum& frusta, const PoolVector<AllocationUnit<CullData> > &in, std::vector<Cullable*>& out);
	}
}

#endif