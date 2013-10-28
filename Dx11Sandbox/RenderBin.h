#ifndef DX11SANDBOX_RenderBin_H
#define DX11SANDBOX_RenderBin_H

#include "CommonUtilities.h"
#include "RenderBinHandler.h"
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
	class RenderObject;
	class CullInfo;
    class RenderBin
    {
    public:

        RenderBin(RenderBinHandler* defaultRenderBinHandler = 0);
        ~RenderBin();
 
        void setDefaultBinRenderBinHandler( RCObjectPtr<RenderBinHandler> RenderBinHandler ); 
       
        void setRenderBinHandlerForBin( RenderQueueID id, RCObjectPtr<RenderBinHandler> RenderBinHandler );
        void appendPrimitivesToBins(std::vector<CullInfo*> &primitives, RenderMask mask);
        void clearBins();

		std::map<RenderQueueID, std::vector<RenderObject*> >& getRenderBins();
		RCObjectPtr<RenderBinHandler> getHandlerForBin(RenderQueueID id);
    private:
        
    

        std::map<RenderQueueID, RCObjectPtr<RenderBinHandler> > m_binHandlers;
        std::map<RenderQueueID, std::vector<RenderObject*> > m_bins;
      
        RCObjectPtr<RenderBinHandler> m_defaultRenderBinHandler;

        
    };
    
}
#endif