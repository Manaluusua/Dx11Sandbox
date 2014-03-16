#include "SIMDCuller.h"
#include "CullData.h"
#include "DynamicPoolAllocator.h"
#include "Frustrum.h"
namespace Dx11Sandbox
{
    

    SIMDCuller::SIMDCuller(void)
    {
    }


    SIMDCuller::~SIMDCuller(void)
    {
    }


    void SIMDCuller::cull(const Frustrum& frusta,PoolVector<AllocationUnit<CullData> > &in ,std::vector<Cullable*>& out)
    {
        Frustrum::SIMDFrustrum simdFrust;
        frusta.convertToSimdFrustrum(simdFrust);

        int maxSphereInd = in.count-1;

        for(int i=0;i<=maxSphereInd;i += 2)
        {

           
            const D3DXVECTOR4 &sphere1 = (*in.vector)[i].data.boundingSphere;
            const D3DXVECTOR4 &sphere2 = (*in.vector)[min(i + 1, maxSphereInd)].data.boundingSphere; 

            UINT32 result = Frustrum::cullSpheresSSE(simdFrust,sphere1, sphere2);

            

            if(result & 0x1)
            {
				Cullable* obj = (*in.vector)[i].data.object;
                out.push_back(obj);
            }
            
            if(  ((i+1 <= maxSphereInd) && (result & 0x2)) )
            {
				Cullable* obj  = (*in.vector)[i+1].data.object;
                out.push_back(obj);
            }

        }

    }

}