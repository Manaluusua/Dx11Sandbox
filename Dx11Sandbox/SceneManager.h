#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "Camera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "DynamicPoolAllocator.h"
#include "RCObjectPtr.h"
#include "CullInfo.h"
#include "RenderBinHandler.h"

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>


namespace Dx11Sandbox
{
    class Material;
    class Renderer;
    class RenderContext;
    class SceneManager;
    class Frustrum;
    class Culler;

    
    
    // renderer listener to listen and act for rendering events
    class RenderStartListener
    {
    public:
        virtual void renderingStarted(RenderContext* context,SceneManager* mngr, double fTime, float fElapsedTime)=0;
        
    };

    

    class SceneManager: public DynamicPoolAllocator<CullInfo>
    {
    friend class Root;
    public:
        
       

        virtual ~SceneManager(void);

        void renderScene( double fTime, float fElapsedTime,  Camera* cam);

        Camera& getMainCamera();
        RenderContext& getRenderContext();

        Renderer* getDefaultRenderer();
        UINT getScreenWidth() const;
        UINT getScreenHeight() const;


        RenderBinHandler& getRenderBinHandler();

        //listeners
        void addRenderStartListener(RenderStartListener* l);
        void removeRenderStartListener(RenderStartListener* l);

        

        


        void cullObjectsToRenderQueues(Frustrum& frust);

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        
       

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

        Camera  m_mainCamera;

        RenderBinHandler    m_renderBinHandler;

        std::set<RenderStartListener*> m_renderStartListeners;

        std::vector<CullInfo*> m_cachedVisibleList;
        
        RCObjectPtr<Culler> m_culler;

        

        Root* m_root;

        RenderContext m_renderContext;

        
        
        UINT m_screenWidth, m_screenHeight;
    private:
        static SceneManager* createSceneManager(Root* root);
        DISABLE_COPY(SceneManager)
        SceneManager(Root* root);



    };

}


#endif
