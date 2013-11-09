#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "RenderCamera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "DynamicPoolAllocator.h"
#include "RCObjectPtr.h"
#include "CullInfo.h"
#include "RenderBin.h"
#include "RenderObjectManager.h"

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>


namespace Dx11Sandbox
{
    class Material;
    class GeometryBinHandler;
    class RenderContext;
    class SceneManager;
    class Frustrum;
    class Culler;

    
    
    // GeometryBinHandler listener to listen and act for rendering events
    class RenderStartListener
    {
    public:
        virtual void renderingStarted(RenderContext* context,SceneManager* mngr, double fTime, float fElapsedTime)=0;
        
    };

    

    class SceneManager 
    {
    friend class Root;
    public:
        
       

        virtual ~SceneManager(void);

        void renderScene( );

        RCObjectPtr<RenderCamera> getMainCamera();
        RenderContext& getRenderContext();

        UINT getScreenWidth() const;
        UINT getScreenHeight() const;


        RenderBin& getRenderBin();

		CullableGeometry* CreateRenderObject();

        //listeners
        void addRenderStartListener(RenderStartListener* l);
        void removeRenderStartListener(RenderStartListener* l);

        
		void addCamera(RCObjectPtr<RenderCamera> camera);
		void removeCamera(RCObjectPtr<RenderCamera> camera);
        


        void cullObjectsToRenderQueues(RCObjectPtr<RenderCamera> cam);

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

		std::vector<RCObjectPtr<RenderCamera>> m_cameras;

        RenderBin    m_RenderBin;

		RenderObjectManager m_renderObjectManager;

        std::set<RenderStartListener*> m_renderStartListeners;

        std::vector<CullInfo*> m_cachedVisibleList;
        
        RCObjectPtr<Culler> m_culler;

        RCObjectPtr<RenderCamera> m_mainCamera;

        Root* m_root;

        RenderContext m_renderContext;

        RCObjectPtr<Renderer> m_defaultRenderer;
        
        UINT m_screenWidth, m_screenHeight;
    private:
        static SceneManager* createSceneManager(Root* root);
        DISABLE_COPY(SceneManager)
        SceneManager(Root* root);



    };

}


#endif
