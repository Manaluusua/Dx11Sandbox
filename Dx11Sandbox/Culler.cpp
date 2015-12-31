#include "Culler.h"
#include "CullData.h"
#include "RenderData.h"
#include "DynamicPoolAllocator.h"
#include "Frustum.h"
namespace Dx11Sandbox
{
    
	namespace Culler
	{
		void cull(const Frustum& frusta, const std::vector<RenderData*>&in, std::vector<RenderData*>& out)
		{
			Frustum::SIMDFrustrum simdFrust;
			frusta.convertToSimdFrustrum(simdFrust);

			for (size_t i = 0; i < in.size(); i += 2)
			{


				const D3DXVECTOR4 &sphere1 = in[i]->getWorldBounds();
				const D3DXVECTOR4 &sphere2 = in[min(i + 1, in.size() - 1)]->getWorldBounds();

				UINT32 result = Frustum::cullSpheresSSE(simdFrust, sphere1, sphere2);



				if (result & 0x1)
				{
					out.push_back(in[i]);
				}

				if ((i + 1 < in.size()) && (result & 0x2))
				{
					out.push_back(in[i + 1]);
				}

			}
		}

		void cull(const Frustum& frusta, const PoolVector<AllocationUnit<CullData> > &in, std::vector<Cullable*>& out)
		{
			Frustum::SIMDFrustrum simdFrust;
			frusta.convertToSimdFrustrum(simdFrust);

			int maxSphereInd = in.count - 1;

			for (int i = 0; i <= maxSphereInd; i += 2)
			{


				const D3DXVECTOR4 &sphere1 = (*in.vector)[i].data.boundingSphere;
				const D3DXVECTOR4 &sphere2 = (*in.vector)[min(i + 1, maxSphereInd)].data.boundingSphere;

				UINT32 result = Frustum::cullSpheresSSE(simdFrust, sphere1, sphere2);



				if (result & 0x1)
				{
					Cullable* obj = (*in.vector)[i].data.object;
					out.push_back(obj);
				}

				if (((i + 1 <= maxSphereInd) && (result & 0x2)))
				{
					Cullable* obj = (*in.vector)[i + 1].data.object;
					out.push_back(obj);
				}

			}

		}
	}
}