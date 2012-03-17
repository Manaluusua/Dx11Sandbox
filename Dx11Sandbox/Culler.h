#ifndef DX11SANDBOX_CULLER_H
#define DX11SANDBOX_CULLER_H

#include <vector>

namespace Dx11Sandbox
{
    class RenderObject;
    class Frustrum;
    template <typename RenderObject> class AllocationUnit;
    template <typename RenderObject> class PoolVector;

    class Culler
    {
    public:
        virtual void cull(const Frustrum frusta,const std::vector<RenderObject>& in ,std::vector<const RenderObject*>& out) =0;
    
        //cull pool allocated objects. Ugly, figure out a more uniform way without sacrifing the theoretic gains from better cache hits :p
        virtual void cull(const Frustrum frusta,const PoolVector<AllocationUnit<RenderObject> > &in ,std::vector<const RenderObject*>& out) =0;
        
    };

}

#endif