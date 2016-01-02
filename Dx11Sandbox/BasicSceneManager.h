#ifndef DX11SANDBOX_BASICSCENEMANAGER_H
#define DX11SANDBOX_BASICSCENEMANAGER_H

#include "SceneManager.h"
#include "RenderCamera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "RCObjectPtr.h"
#include "CullData.h"
#include "RenderBin.h"
#include "CullableGeometry.h"
#include "CullableObjectManager.h"
#include "EnvironmentChangedListeners.h"
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
    class Frustum;
	class DebugDrawer;
    
    
    // GeometryBinHandler listener to listen and act for rendering events
    class RenderStartListener
    {
    public:
        virtual void renderingStarted(RenderContext* context,SceneManager* mngr, double fTime, float fElapsedTime)=0;
        
    };

    
	//Default implementation of a scenemanager
    class BasicSceneManager : public SceneManager
    {
    friend class Root;
    public:
        
       

		virtual ~BasicSceneManager(void);

		virtual void renderScene();

		virtual RenderCamera* getMainCamera();
		virtual RenderContext& getRenderContext();


		virtual RenderBin& getRenderBin();

		virtual CullableGeometry* createCullableGeometry();
		virtual CullableLight* createLight();

		virtual void addDebugDrawer(DebugDrawer* drawer);
		virtual void removeDebugDrawer(DebugDrawer* drawer);

		virtual void addEnvironmentListener(EnvironmentChangedListeners* l);
		virtual void removeEnvironmentListener(EnvironmentChangedListeners* l);
        
		virtual RenderCamera* createCamera();
		virtual void destroyCamera(RenderCamera* camera);
        
		virtual void calculateVisibleObjectsForCamera(RenderCamera* cam, std::vector<Cullable*>& out);

		virtual void renderCamera(RenderCamera* cam);

    protected:
		virtual void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
		virtual void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
		virtual void update(double fTime, float fElapsedTime);
		virtual void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        
		void drawDebug();

		void cullObjectsToRenderQueues(RenderCamera* cam) ;
		void cullObjectsFromPools(std::map<RenderLayer, CullDataAllocator*>& pools, RenderCamera* cam, std::vector<Cullable*>& out);
		void addCachedObjectsToRenderBins();

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

		std::vector<RenderCamera*> m_cameras;

        RenderBin    m_RenderBin;

		CullableObjectManager m_cullableObjectManager;
		std::vector<Cullable*> m_cachedVisibleObjectsList;
		std::vector<DebugDrawer*> m_debugDrawList;
        
		std::set<EnvironmentChangedListeners*> m_environmentListeners;

        RenderCamera* m_mainCamera;

        Root* m_root;

        RenderContext m_renderContext;

        RCObjectPtr<Renderer> m_defaultRenderer;

    private:
        static SceneManager* createSceneManager(Root* root);
		DISABLE_COPY(BasicSceneManager)
		BasicSceneManager(Root* root);



    };

}


#endif
