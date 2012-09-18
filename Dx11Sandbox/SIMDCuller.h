#ifndef DX11SANDBOX_SIMDCULLER_H
#define DX11SANDBOX_SIMDCULLER_H

#include "culler.h"
namespace Dx11Sandbox
{
    class SIMDCuller :
        public Culler
    {
    public:
        SIMDCuller(void);
        virtual ~SIMDCuller(void);
    
        virtual void cull(const Frustrum& frusta,const PoolVector<AllocationUnit<CullInfo> > &in ,std::vector<const CullInfo*>& out);
    };



}

#endif