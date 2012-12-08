#ifndef DX11SANDBOX_RENDERBINHANDLER_H
#define DX11SANDBOX_RENDERBINHANDLER_H

#include "CommonUtilities.h"
#include "CullInfo.h"
#include "Renderer.h"
#include "RCObjectPtr.h"
#include <map>
#include <vector>
#include <string>



namespace Dx11Sandbox
{



    class RenderContext;
    class Camera;
    class SceneManager;
    class RenderBinHandler
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

        

        RenderBinHandler(Renderer* defaultRenderer = 0);
        ~RenderBinHandler();
 
        void setDefaultBinRenderer( RCObjectPtr<Renderer> renderer ); 
        void setRenderPriorityForBin( int priority, IDTYPE binID );
        IDTYPE getIDForBinName( const NAMETYPE& name ) const;
        IDTYPE setRendererForBinWithName(const NAMETYPE& name, RCObjectPtr<Renderer> renderer );  
        void setRendererForBinWithID( IDTYPE id, RCObjectPtr<Renderer> renderer );
        void appendPrimitivesToBins(std::vector<PRIMITIVETYPE*> &primitives);
        void renderBin( IDTYPE binID, RenderContext* context, Camera* camera );
        void renderAllBins(RenderContext* context, Camera* camera);
        void renderBinsUpToPriority( int priority, RenderContext* context, Camera* camera );
        int getPriorityOfRenderBin( IDTYPE id );
        void clearBins();

        void addRenderBinListener(RenderBinListener* l);
        void removeRenderBinListener( RenderBinListener* l);

    private:
        
        static IDTYPE m_nextBinID;

        std::map<IDTYPE, RCObjectPtr<Renderer> > m_binRenderers;
        std::map<IDTYPE, std::vector<PRIMITIVETYPE*> > m_bins;
        mutable std::map<NAMETYPE, IDTYPE> m_nameToIDMap;
        std::vector<IDTYPE> m_binRenderingOrder;
        RCObjectPtr<Renderer> m_defaultRenderer;

        std::vector<RenderBinListener*> m_listeners;

        static IDTYPE getNextBinID();
    };

    inline RenderBinHandler::IDTYPE RenderBinHandler::getNextBinID()
    {
        return m_nextBinID++;
    }
    
}
#endif