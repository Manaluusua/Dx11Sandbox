#include "RenderBin.h"
#include "CullableGeometry.h"
#include "TerrainBinHandler.h"
#include "CullableLight.h"

namespace Dx11Sandbox
{

	//renderbin
    RenderBin::RenderBin(GeometryBinHandler* defaultGeometryBinHandler)
        :m_defaultGeometryBinHandler(defaultGeometryBinHandler)
    {
		m_geometryBinHandlers[RENDERQUEUE_TERRAIN] = new TerrainBinHandler;
    }

    RenderBin::~RenderBin()
    {
    }


    void RenderBin::setDefaultGeometryBinHandler( RCObjectPtr<GeometryBinHandler> rbh )
    {
        m_defaultGeometryBinHandler = rbh;
    }

  

   
    void RenderBin::setRenderBinHandlerForGeometryBin( RenderQueueID id, RCObjectPtr<GeometryBinHandler> handler )
    {
        if(handler.rawPtr() )
        {
            m_geometryBinHandlers[id] = handler;
        }
    }
   

	void RenderBin::addGeometry(Geometry* geom)
	{
		RenderQueueID id = geom->getRenderQueue();
		m_geometryBins[id].push_back( geom );
	}

	void RenderBin::addLight(Light* light)
	{
		m_lights.push_back(light);
	}

	std::vector<Light*>& RenderBin::getLights()
	{
		return m_lights;
	}

	std::map<RenderQueueID, std::vector<RenderData*> >& RenderBin::getGeometryBins()
	{
		return m_geometryBins;
	}

	RCObjectPtr<GeometryBinHandler> RenderBin::getGeometryHandlerForBin(RenderQueueID id)
	{
		RCObjectPtr<GeometryBinHandler> renderBinHandler = m_defaultGeometryBinHandler;

        //choose GeometryBinHandler
        if( m_geometryBinHandlers.find( id ) != m_geometryBinHandlers.end() )
        {
            renderBinHandler = m_geometryBinHandlers[id];
        }
		return renderBinHandler;
	}

 
    void RenderBin::clearBins()
    {
        auto iter = m_geometryBins.begin();
        while( iter != m_geometryBins.end() )
        {
            iter->second.clear();
            ++iter;
        }
        
		m_lights.clear();
    }
}