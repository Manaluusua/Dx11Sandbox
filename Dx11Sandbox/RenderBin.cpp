#include "RenderBin.h"

namespace Dx11Sandbox
{
    const std::string RenderBin::RENDERBIN_DEFAULT = "DEFAULT";
    const std::string RenderBin::RENDERBIN_SKYBOX = "SKYBOX";
    const std::string RenderBin::RENDERBIN_TRANSPARENT = "TRANSPARENT";
    const std::string RenderBin::RENDERBIN_SCENEINPUT = "SCENEINPUT";

    RenderBin::IDTYPE RenderBin::m_nextBinID = 0;

    RenderBin::RenderBin(RenderBinHandler* defaultRenderBinHandler)
        :m_defaultRenderBinHandler(defaultRenderBinHandler)
    {
        IDTYPE defaultRB = getIDForBinName( RENDERBIN_DEFAULT );
        IDTYPE skyboxRB = getIDForBinName( RENDERBIN_SKYBOX );
        IDTYPE transparentRB = getIDForBinName( RENDERBIN_TRANSPARENT );
        IDTYPE sceneInputRB = getIDForBinName( RENDERBIN_SCENEINPUT );

        setRenderPriorityForBin( 0,defaultRB);
        setRenderPriorityForBin( 1,transparentRB);
        setRenderPriorityForBin( 2,sceneInputRB);
        setRenderPriorityForBin( 3,skyboxRB);
    }

    RenderBin::~RenderBin()
    {
    }


    void RenderBin::setDefaultBinRenderBinHandler( RCObjectPtr<RenderBinHandler> rbh )
    {
        m_defaultRenderBinHandler = rbh;
    }

    void RenderBin::setRenderPriorityForBin( int priority, RenderBin::IDTYPE binID )
    {
        for( int i = 0; i < m_binRenderingOrder.size(); ++i )
        {
            if(m_binRenderingOrder[i] == binID)
            {
                m_binRenderingOrder.erase( m_binRenderingOrder.begin() + i );
                break;
            }
        }

        if( priority >= m_binRenderingOrder.size() )
        {
            m_binRenderingOrder.push_back( binID );
        }else
        {
            m_binRenderingOrder.insert( m_binRenderingOrder.begin() + priority, binID );
        }

    }

    RenderBin::IDTYPE RenderBin::getIDForBinName( const RenderBin::NAMETYPE& name ) const
    {
        IDTYPE ID = 0;
        if( m_nameToIDMap.find( name ) == m_nameToIDMap.end() )
        {
            ID = getNextBinID();
            m_nameToIDMap.insert( std::make_pair< NAMETYPE, IDTYPE >( name, ID ) );
        }else
        {
            ID = m_nameToIDMap[name];
        }

        return ID;
    }

    RenderBin::IDTYPE RenderBin::setRenderBinHandlerForBinWithName( const RenderBin::NAMETYPE& name, RCObjectPtr<RenderBinHandler> handler )
    {
        IDTYPE ID = getIDForBinName( name );

        setRenderBinHandlerForBinWithID( ID, handler );
        return ID;
    }
    void RenderBin::setRenderBinHandlerForBinWithID( RenderBin::IDTYPE id, RCObjectPtr<RenderBinHandler> handler )
    {
        if(handler.rawPtr() )
        {
            m_binHandlers[id] = handler;
        }
    }
    void RenderBin::appendPrimitivesToBins(std::vector<RenderBin::PRIMITIVETYPE*> &primitives, RenderMask mask)
    {
        for( int i = 0; i < primitives.size(); ++i )
        {
			RenderBin::PRIMITIVETYPE* primitive = primitives[i];
			if(mask & primitive->flags)
			{
				m_bins[primitives[i]->binIDFlag].push_back( primitives[i] );
			}
            
        }
    }
    void RenderBin::renderBin( RenderBin::IDTYPE binID, RenderContext* context, Camera* camera )
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

    void RenderBin::addRenderBinListener(RenderBinListener* l)
    {
        m_listeners.push_back( l );
    }

    void RenderBin::removeRenderBinListener( RenderBinListener* l)
    {
        for( int i = 0; i < m_listeners.size(); ++i )
        {
            if(m_listeners[i] == l )
            {
                m_listeners.erase(m_listeners.begin() + i );
                --i;
            }
        }
    }


    void RenderBin::renderBinsUpToPriority( int priority,RenderContext* context, Camera* camera )
    {
        int upTo = priority < m_binRenderingOrder.size() ? priority : m_binRenderingOrder.size() - 1;

        for( int i = 0;i <= upTo ; ++i )
        {
            renderBin( m_binRenderingOrder[i], context, camera );
        }
    }

    int RenderBin::getPriorityOfRenderBin( IDTYPE id )
    {
        int result = -1;
        for( int i = 0; i < m_binRenderingOrder.size(); ++i )
        {
            if(m_binRenderingOrder[i] == id)
            {
               result = i;
               break;
            }
        }
        return result;
    }

    void RenderBin::renderAllBins( RenderContext* context, Camera* camera)
    {
        for( int i = 0;i < m_binRenderingOrder.size(); ++i )
        {
            renderBin( m_binRenderingOrder[i], context, camera );
        }
    }
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