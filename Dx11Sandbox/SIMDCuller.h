#ifndef DX11SANDBOX_SIMDCULLER_H
#define DX11SANDBOX_SIMDCULLER_H

#include "Culler.h"
namespace Dx11Sandbox
{
    class SIMDCuller :
        public Culler
    {
    public:
        SIMDCuller(void);
        virtual ~SIMDCuller(void);
    
        virtual void cull(const Frustum& frusta,PoolVector<AllocationUnit<CullData> > &in ,std::vector<Cullable*>& out);
    };



}

#endif