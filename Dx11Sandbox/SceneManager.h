#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "RenderCamera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "RCObjectPtr.h"
#include "CullData.h"
#include "RenderBin.h"
#include "CullableGeometry.h"
#include "CullableObjectManager.h"
#include "CullableLight.h"
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

        RenderCamera* getMainCamera();
        RenderContext& getRenderContext();


        RenderBin& getRenderBin();

		CullableGeometry* createCullableGeometry();
		CullableLight* createLight();

        //listeners
        void addRenderStartListener(RenderStartListener* l);
        void removeRenderStartListener(RenderStartListener* l);

        
		RenderCamera* createCamera();
		void destroyCamera(RenderCamera* camera);
        


        void cullObjectsToRenderQueues(RenderCamera* cam);
		

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        

		void cullObjectsFromPools(std::map<RenderLayer, CullDataAllocator*>& pools, RenderCamera* cam);

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

		std::vector<RenderCamera*> m_cameras;

        RenderBin    m_RenderBin;

		CullableGeometryManager m_renderGeometryManager;
		CullableLightManager m_lightManager;

        std::set<RenderStartListener*> m_renderStartListeners;
        std::vector<Cullable*> m_cachedVisibleList;
        
        RCObjectPtr<Culler> m_culler;

        RenderCamera* m_mainCamera;

        Root* m_root;

        RenderContext m_renderContext;

        RCObjectPtr<Renderer> m_defaultRenderer;

    private:
        static SceneManager* createSceneManager(Root* root);
        DISABLE_COPY(SceneManager)
        SceneManager(Root* root);



    };

}


#endif
