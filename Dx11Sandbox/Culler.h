#ifndef DX11SANDBOX_CULLER_H
#define DX11SANDBOX_CULLER_H

#include "RCObject.h"
#include <vector>

namespace Dx11Sandbox
{
    class CullData;
	class Cullable;
    class Frustrum;
    template <typename CullData> class AllocationUnit;
    template <typename CullData> class PoolVector;

    class Culler: public RCObject
    {
    public:

    
        virtual void cull(const Frustrum& frusta,PoolVector<AllocationUnit<CullData> > &in ,std::vector<Cullable*>& out) =0;
		
        
    };

}

#endif