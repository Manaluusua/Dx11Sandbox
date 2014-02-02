#ifndef DX11SANDBOX_RENDERBIN_H
#define DX11SANDBOX_RENDERBIN_H

#include "RenderPrimitiveGatherer.h"
#include "CommonUtilities.h"
#include "GeometryBinHandler.h"
#include "RCObjectPtr.h"
#include <map>
#include <vector>
#include <string>



namespace Dx11Sandbox
{

    class RenderContext;
    class Camera;
    class SceneManager;
	class Light;
	class RenderData;
	class CullableGeometry;
	class Cullable;
    class RenderBin : public RenderPrimitiveGatherer
    {
    public:

        RenderBin(GeometryBinHandler* defaultGeometryBinHandler = 0);
        ~RenderBin();
		
		virtual void addGeometry(Geometry* geom);
		virtual void addLight(Light* light);
        void clearBins();

        void setDefaultGeometryBinHandler( RCObjectPtr<GeometryBinHandler> GeometryBinHandler ); 
        void setRenderBinHandlerForGeometryBin( RenderQueueID id, RCObjectPtr<GeometryBinHandler> geometryBinHandler );
        
		std::map<RenderQueueID, std::vector<RenderData*> >& getGeometryBins();
		RCObjectPtr<GeometryBinHandler> getGeometryHandlerForBin(RenderQueueID id);

		std::vector<Light*>& getLights();
    private:

        std::map<RenderQueueID, RCObjectPtr<GeometryBinHandler> > m_geometryBinHandlers;
        std::map<RenderQueueID, std::vector<RenderData*> > m_geometryBins;
		
        RCObjectPtr<GeometryBinHandler> m_defaultGeometryBinHandler;

        std::vector<Light*> m_lights;
    };
    
}
#endif