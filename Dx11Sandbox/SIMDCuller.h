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

        virtual void cull(const Frustrum frusta,const std::vector<RenderObject>& in ,std::vector<const RenderObject*>& out);
    
        //cull pool allocated objects. Ugly, figure out a more uniform way without sacrifing the theoretic gains from better cache hits :p
        virtual void cull(const Frustrum frusta,const PoolVector<AllocationUnit<RenderObject> > &in ,std::vector<const RenderObject*>& out);
    };



}

#endif