#include "RenderBin.h"
#include "RenderObject.h"
#include "TerrainBinHandler.h"
namespace Dx11Sandbox
{

	//renderbin
    RenderBin::RenderBin(RenderBinHandler* defaultRenderBinHandler)
        :m_defaultRenderBinHandler(defaultRenderBinHandler)
    {
		m_binHandlers[RENDERQUEUE_TERRAIN] = new TerrainBinHandler;
    }

    RenderBin::~RenderBin()
    {
    }


    void RenderBin::setDefaultBinRenderBinHandler( RCObjectPtr<RenderBinHandler> rbh )
    {
        m_defaultRenderBinHandler = rbh;
    }

  

   
    void RenderBin::setRenderBinHandlerForBin( RenderQueueID id, RCObjectPtr<RenderBinHandler> handler )
    {
        if(handler.rawPtr() )
        {
            m_binHandlers[id] = handler;
        }
    }
    void RenderBin::appendPrimitivesToBins(std::vector<CullInfo*> &primitives, RenderMask mask)
    {
        for( unsigned int i = 0; i < primitives.size(); ++i )
        {
			CullInfo* primitive = primitives[i];
			if(mask & primitive->renderMask)
			{
				RenderObject* obj = primitive->object;
				RenderQueueID id = obj->getRenderQueue();
				
				m_bins[id].push_back( obj );
			}
            
        }
    }

	std::map<RenderQueueID, std::vector<RenderObject*> >& RenderBin::getRenderBins()
	{
		return m_bins;
	}

	RCObjectPtr<RenderBinHandler> RenderBin::getHandlerForBin(RenderQueueID id)
	{
		RCObjectPtr<RenderBinHandler> renderBinHandler = m_defaultRenderBinHandler;

        //choose RenderBinHandler
        if( m_binHandlers.find( id ) != m_binHandlers.end() )
        {
            renderBinHandler = m_binHandlers[id];
        }
		return renderBinHandler;
	}

   /* void RenderBin::renderBin( RenderBin::IDTYPE binID, RenderContext* context, Camera* camera )
    {
        RenderBinHandler* RenderBinHandler = m_defaultRenderBinHandler;



        
        //choose RenderBinHandler
        if( m_binHandlers.find( binID ) != m_binHandlers.end() )
        {
            RenderBinHandler = m_binHandlers[binID];
        }

        //get primitives
        std::vector<PRIMITIVETYPE*>& prims = m_bins[binID];

        //notify listeners
        for( int i = 0; i < m_listeners.size(); ++i )
        {
            m_listeners[i]->renderingBin( prims, context );
        }

        //render
        if( prims.size() > 0 )
        {
            RenderBinHandler->render( prims, context, camera );
        }

    }
	*/
    void RenderBin::clearBins()
    {
        auto iter = m_bins.begin();
        while( iter != m_bins.end() )
        {
            iter->second.clear();
            ++iter;
        }
        
    }
}