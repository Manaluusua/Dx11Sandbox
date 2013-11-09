#ifndef DX11SANDBOX_RenderBin_H
#define DX11SANDBOX_RenderBin_H

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
	class RenderData;
	class CullableGeometry;
	class CullInfo;
    class RenderBin
    {
    public:

        RenderBin(GeometryBinHandler* defaultGeometryBinHandler = 0);
        ~RenderBin();
		
		void appendPrimitives(std::vector<CullInfo*> &primitives, RenderMask mask);
        void clearBins();

        void setDefaultGeometryBinHandler( RCObjectPtr<GeometryBinHandler> GeometryBinHandler ); 
        void setRenderBinHandlerForGeometryBin( RenderQueueID id, RCObjectPtr<GeometryBinHandler> geometryBinHandler );
        
		std::map<RenderQueueID, std::vector<RenderData*> >& getGeometryBins();
		RCObjectPtr<GeometryBinHandler> getGeometryHandlerForBin(RenderQueueID id);


    private:
        
    

        std::map<RenderQueueID, RCObjectPtr<GeometryBinHandler> > m_geometryBinHandlers;
        std::map<RenderQueueID, std::vector<RenderData*> > m_geometryBins;
		
        RCObjectPtr<GeometryBinHandler> m_defaultGeometryBinHandler;

        
    };
    
}
#endif