#ifndef DX11SANDBOX_RenderBin_H
#define DX11SANDBOX_RenderBin_H

#include "CommonUtilities.h"
#include "CullInfo.h"
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
    class RenderBin
    {
    public:



        typedef int IDTYPE;
        typedef std::string NAMETYPE;
        typedef CullInfo PRIMITIVETYPE;

        //some basic rendering bins
        static const std::string RENDERBIN_DEFAULT;
        static const std::string RENDERBIN_SKYBOX;
        static const std::string RENDERBIN_TRANSPARENT;
        static const std::string RENDERBIN_SCENEINPUT;

        class RenderBinListener
        {
        public:
            virtual void renderingBin(std::vector< PRIMITIVETYPE*> &primitives, RenderContext* state)=0;
        };

        

        RenderBin(RenderBinHandler* defaultRenderBinHandler = 0);
        ~RenderBin();
 
        void setDefaultBinRenderBinHandler( RCObjectPtr<RenderBinHandler> RenderBinHandler ); 
        void setRenderPriorityForBin( int priority, IDTYPE binID );
        IDTYPE getIDForBinName( const NAMETYPE& name ) const;
        IDTYPE setRenderBinHandlerForBinWithName(const NAMETYPE& name, RCObjectPtr<RenderBinHandler> RenderBinHandler );  
        void setRenderBinHandlerForBinWithID( IDTYPE id, RCObjectPtr<RenderBinHandler> RenderBinHandler );
        void appendPrimitivesToBins(std::vector<PRIMITIVETYPE*> &primitives, RenderMask mask);
        void renderBin( IDTYPE binID, RenderContext* context, Camera* camera );
        void renderAllBins(RenderContext* context, Camera* camera);
        void renderBinsUpToPriority( int priority, RenderContext* context, Camera* camera );
        int getPriorityOfRenderBin( IDTYPE id );
        void clearBins();

        void addRenderBinListener(RenderBinListener* l);
        void removeRenderBinListener( RenderBinListener* l);

    private:
        
        static IDTYPE m_nextBinID;

        std::map<IDTYPE, RCObjectPtr<RenderBinHandler> > m_binHandlers;
        std::map<IDTYPE, std::vector<PRIMITIVETYPE*> > m_bins;
        mutable std::map<NAMETYPE, IDTYPE> m_nameToIDMap;
        std::vector<IDTYPE> m_binRenderingOrder;
        RCObjectPtr<RenderBinHandler> m_defaultRenderBinHandler;

        std::vector<RenderBinListener*> m_listeners;

        static IDTYPE getNextBinID();
    };

    inline RenderBin::IDTYPE RenderBin::getNextBinID()
    {
        return m_nextBinID++;
    }
    
}
#endif