#include "SceneManager.h"
#include "Root.h"
#include "TextureManager.h"
#include "CullInfoManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "BasicBinHandler.h"
#include "Frustrum.h"
#include "CullInfo.h"
#include "SIMDCuller.h"
#include <algorithm>
namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_RenderBin( new BasicBinHandler() ),
        m_renderContext(),
        m_culler(new SIMDCuller()),
        m_screenWidth(0),
        m_screenHeight(0)
    {
        m_mainCamera = new RenderCamera;
		addCamera(m_mainCamera);

    }

    SceneManager* SceneManager::createSceneManager(Root* root)
    {
        SceneManager* mngr = new SceneManager(root);

        return mngr;
    }


    SceneManager::~SceneManager(void)
    {
        destroyWorld();
        destroyManagers();
    }

    void SceneManager::destroyManagers()
    {
		TextureManager::destroyInstance();
        MaterialManager::destroyInstance();
        MeshManager::destroyInstance();
		
		CullInfoManager::destroyInstance();
		
    }

    void SceneManager::addRenderStartListener(RenderStartListener* l)
    {
        m_renderStartListeners.insert(l);
    }

    void SceneManager::removeRenderStartListener(RenderStartListener* l)
    {
        m_renderStartListeners.erase(l);
    }

	void addCamera(RCObjectPtr<Camera> camera);
		void removeCamera(RCObjectPtr<Camera> camera);

    RCObjectPtr<RenderCamera> SceneManager::getMainCamera()
    {
        return m_mainCamera;
    }

	void SceneManager::addCamera(RCObjectPtr<RenderCamera> camera){
		if(camera == 0) return;

		m_cameras.push_back(camera);

	}


	void SceneManager::removeCamera(RCObjectPtr<RenderCamera> camera){
		if(camera == 0) return;

		for(unsigned int i = 0; i < m_cameras.size(); ++i)
		{
			if(m_cameras[i].rawPtr() == camera.rawPtr()){
				m_cameras.erase(m_cameras.begin() + i );
				break;
			}
		}

	}

    RenderBin& SceneManager::getRenderBin()
    {
        return m_RenderBin;
    }

    RenderContext& SceneManager::getRenderContext()
    {
        return m_renderContext;
    }


    UINT SceneManager::getScreenWidth() const
    {
        return m_screenWidth;
    }
    UINT SceneManager::getScreenHeight() const
    {
        return m_screenHeight;
    }


    void SceneManager::windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
         // Setup the camera's projection parameters
        float aspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
        m_mainCamera->setProjection(D3DX_PI / 4, aspectRatio, 0.1f, 800.0f);

        m_screenWidth = pBackBufferSurfaceDesc->Width;
        m_screenHeight = pBackBufferSurfaceDesc->Height;
        
    }




    void SceneManager::initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
        m_screenWidth = pBackBufferSurfaceDesc->Width;
        m_screenHeight = pBackBufferSurfaceDesc->Height;
        m_renderContext.setDevice(pd3dDevice);
    }

    void SceneManager::destroyWorld()
    {
		DestroyAllRenderObjects();
        clearRenderQueues();
		CullInfoManager::singleton()->deallocateAll();
    }

    void SceneManager::clearRenderQueues()
    {
        m_RenderBin.clearBins();
    }

    void SceneManager::update(double fTime, float fElapsedTime)
    {
        
       
    }


    void SceneManager::beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime)
    {

        m_renderContext.setDevice(pd3dDevice);
        m_renderContext.setImmediateContext(pd3dImmediateContext);
        

        //first notify all listeners and let them render what they need
        std::set<RenderStartListener*>::iterator it = m_renderStartListeners.begin();
        while(it != m_renderStartListeners.end())
        {
            (*it)->renderingStarted( &m_renderContext, this, fTime, fElapsedTime);
            ++it;
        }
        // Clear render target and the depth stencil 
        float ClearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
        //pd3dImmediateContext->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), ClearColor );
        pd3dImmediateContext->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );

        renderScene();

    }

    void SceneManager::renderScene()
    {
        
        m_renderContext.clearState();

		//iterate through camera
		
		std::sort(m_cameras.begin(), m_cameras.end(), [] ( RCObjectPtr<RenderCamera>& cam1, RCObjectPtr<RenderCamera>& cam2 )
		{
			return cam1->getCameraPriority() <= cam2->getCameraPriority();
		});


		for( unsigned int i = 0; i < m_cameras.size(); ++i)
		{
			RCObjectPtr<RenderCamera> cam = m_cameras[i];
			cullObjectsToRenderQueues(cam);
			
				
		}


    }


	


    void SceneManager::cullObjectsToRenderQueues(RCObjectPtr<RenderCamera> cam)
    {
		Frustrum frust;
		cam->calculateFrustrum(&frust);

        clearRenderQueues();

        m_cachedVisibleList.clear();

		for( unsigned int i=0;i<CullInfoManager::singleton()->getNumberOfPools();++i)
        {

			CullInfoPool &objects = CullInfoManager::singleton()->getCullInfoPool(i);
            m_culler->cull(frust,objects,m_cachedVisibleList);
        }

		m_RenderBin.appendPrimitivesToBins( m_cachedVisibleList, cam->getRenderMask() );


    }
}