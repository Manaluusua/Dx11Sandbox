#ifndef DX11SANDBOX_CULLER_H
#define DX11SANDBOX_CULLER_H

#include "RCObject.h"
#include <vector>

namespace Dx11Sandbox
{
    class CullInfo;
    class Frustrum;
    template <typename CullInfo> class AllocationUnit;
    template <typename CullInfo> class PoolVector;

    class Culler: public RCObject
    {
    public:

    
        virtual void cull(const Frustrum& frusta,PoolVector<AllocationUnit<CullInfo> > &in ,std::vector<CullInfo*>& out) =0;
        
    };

}

#endif