#include "SIMDCuller.h"
#include "SceneManager.h"
namespace Dx11Sandbox
{

    SIMDCuller::SIMDCuller(void)
    {
    }


    SIMDCuller::~SIMDCuller(void)
    {
    }

    void SIMDCuller::cull(const Frustrum frusta,const std::vector<RenderObject>& in ,std::vector<const RenderObject*>& out)
    {
        for(int i=0;i<in.size();++i)
        {
            const RenderObject* ro = &in[i];
            out.push_back(ro);
        }
    }
    
    //cull pool allocated objects. Ugly, figure out a more uniform way without sacrifing the theoretic gains from better cache hits :p
    void SIMDCuller::cull(const Frustrum frusta,const PoolVector<AllocationUnit<RenderObject> > &in ,std::vector<const RenderObject*>& out)
    {
        for(int i=0;i<in.count;++i)
        {
            out.push_back(&in.vector[i].data);
        }
    }
}