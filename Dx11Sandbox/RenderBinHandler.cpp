#include "RenderBinHandler.h"

namespace Dx11Sandbox
{


    RenderBinHandler::IDTYPE RenderBinHandler::m_nextBinID = 0;

    RenderBinHandler::RenderBinHandler(Renderer* defaultRenderer)
        :m_defaultRenderer(defaultRenderer)
    {
        IDTYPE defaultRB = getIDForBinName( RENDERBIN_DEFAULT );
        IDTYPE skyboxRB = getIDForBinName( RENDERBIN_SKYBOX );
        IDTYPE transparentRB = getIDForBinName( RENDERBIN_TRANSPARENT );
        IDTYPE sceneInputRB = getIDForBinName( RENDERBIN_SCENEINPUT );

        setRenderPriorityForBin( 0,defaultRB);
        setRenderPriorityForBin( 1,skyboxRB);
        setRenderPriorityForBin( 2,transparentRB);
        setRenderPriorityForBin( 3,sceneInputRB);
    }

    RenderBinHandler::~RenderBinHandler()
    {
    }


    void RenderBinHandler::setDefaultBinRenderer( RCObjectPtr<Renderer> renderer )
    {
        m_defaultRenderer = renderer;
    }

    void RenderBinHandler::setRenderPriorityForBin( int priority, RenderBinHandler::IDTYPE binID )
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

    RenderBinHandler::IDTYPE RenderBinHandler::getIDForBinName( const RenderBinHandler::NAMETYPE& name ) const
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

    RenderBinHandler::IDTYPE RenderBinHandler::setRendererForBinWithName( const RenderBinHandler::NAMETYPE& name, RCObjectPtr<Renderer> renderer )
    {
        IDTYPE ID = getIDForBinName( name );

        setRendererForBinWithID( ID, renderer );
        return ID;
    }
    void RenderBinHandler::setRendererForBinWithID( RenderBinHandler::IDTYPE id, RCObjectPtr<Renderer> renderer )
    {
        if(renderer.rawPtr() )
        {
            m_binRenderers[id] = renderer;
        }
    }
    void RenderBinHandler::appendPrimitivesToBins(std::vector<RenderBinHandler::PRIMITIVETYPE*> &primitives)
    {
        for( int i = 0; i < primitives.size(); ++i )
        {
            m_bins[primitives[i]->binIDFlag].push_back( primitives[i] );
        }
    }
    void RenderBinHandler::renderBin( RenderBinHandler::IDTYPE binID, RenderContext* context, Camera* camera )
    {
        Renderer* renderer = m_defaultRenderer;



        
        //choose renderer
        if( m_binRenderers.find( binID ) != m_binRenderers.end() )
        {
            renderer = m_binRenderers[binID];
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
            renderer->render( prims, context, camera );
        }

    }

    void RenderBinHandler::addRenderBinListener(RenderBinListener* l)
    {
        m_listeners.push_back( l );
    }

    void RenderBinHandler::removeRenderBinListener( RenderBinListener* l)
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


    void RenderBinHandler::renderBinsUpToPriority( int priority,RenderContext* context, Camera* camera )
    {
        int upTo = priority < m_binRenderingOrder.size() ? priority : m_binRenderingOrder.size() - 1;

        for( int i = 0;i <= upTo ; ++i )
        {
            renderBin( m_binRenderingOrder[i], context, camera );
        }
    }

    int RenderBinHandler::getPriorityOfRenderBin( IDTYPE id )
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

    void RenderBinHandler::renderAllBins( RenderContext* context, Camera* camera)
    {
        for( int i = 0;i < m_binRenderingOrder.size(); ++i )
        {
            renderBin( m_binRenderingOrder[i], context, camera );
        }
    }
    void RenderBinHandler::clearBins()
    {
        auto iter = m_bins.begin();
        while( iter != m_bins.end() )
        {
            iter->second.clear();
            ++iter;
        }
        
    }
}