#include "SceneManager.h"
#include "Root.h"
#include "TextureManager.h"
#include "CullDataAllocator.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "BasicBinHandler.h"
#include "Frustrum.h"
#include "CullData.h"
#include "SIMDCuller.h"
#include "BasicForwardRenderer.h"
#include "DeferredRenderer.h"
#include "EnvironmentInfo.h"
#include "DebugDrawer.h"
#include <algorithm>

namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_RenderBin( new BasicBinHandler() ),
        m_renderContext(),
        m_culler(new SIMDCuller())
    {

		EnvironmentInfo::m_accumulatedTime = 0;
		EnvironmentInfo::m_deltaTime = 0;

		TextureManager::singleton()->setAssetPath(root->GetMediaPath());
		MaterialManager::singleton()->setAssetPath(root->GetMediaPath());

		m_defaultRenderer = new BasicForwardRenderer;
		m_mainCamera = createCamera();
		m_mainCamera->setRenderer(new DeferredRenderer(&m_renderContext));
		m_mainCamera->setRenderMask(0xFFFF);
		

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
		
		
    }

	CullableGeometry* SceneManager::createCullableGeometry()
	{
		return m_renderGeometryManager.create();
	}

	CullableLight* SceneManager::createLight()
	{
		return m_lightManager.create();
	}


	void SceneManager::addDebugDrawer(DebugDrawer* drawer)
	{
		if(drawer == 0) return;

		m_debugDrawList.push_back(drawer);
	}

	void SceneManager::removeDebugDrawer(DebugDrawer* drawer)
	{
		if(drawer == 0) return;

		for( unsigned int i = 0; i < m_debugDrawList.size(); ++i)
		{
			if(m_debugDrawList[i] != drawer) continue;
			m_debugDrawList.erase(m_debugDrawList.begin() + i);
			--i;
		}

	}


	void SceneManager::addEnvironmentListener(EnvironmentChangedListeners* l)
	{
		m_environmentListeners.insert(l);
	}
	void SceneManager::removeEnvironmentListener(EnvironmentChangedListeners* l)
	{
		m_environmentListeners.erase(l);
	}

	void SceneManager::drawDebug()
	{
		std::for_each(m_debugDrawList.begin(), m_debugDrawList.end(), [this](DebugDrawer* d) {d->draw(&m_renderContext);});
	}

	RenderCamera* SceneManager::getMainCamera()
    {
        return m_mainCamera;
    }

	RenderCamera* SceneManager::createCamera()
	{
		RenderCamera* cam = new RenderCamera;
		m_cameras.push_back(cam);

		cam->setRenderer(m_defaultRenderer);
		return cam;
	}



	void SceneManager::destroyCamera(RenderCamera* camera){
		if(camera == 0) return;

		for(unsigned int i = 0; i < m_cameras.size(); ++i)
		{
			if(m_cameras[i] == camera){
				m_cameras.erase(m_cameras.begin() + i );
				delete camera;
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




    void SceneManager::windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
         // Setup the camera's projection parameters
        float aspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;

        m_mainCamera->setProjectionPerspective(D3DX_PI / 3, aspectRatio, 0.1f, 800.0f);

		//setup new viewport
		D3D11_VIEWPORT vp;
		vp.MinDepth = 0.f;
		vp.MaxDepth = 1.f;
		vp.Width = pBackBufferSurfaceDesc->Width;
		vp.Height = pBackBufferSurfaceDesc->Height;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

		m_renderContext.setDefaultViewport(&vp);

		EnvironmentInfo::m_screenWidth = pBackBufferSurfaceDesc->Width;
        EnvironmentInfo::m_screenHeight = pBackBufferSurfaceDesc->Height;
        
		for( auto iter = m_environmentListeners.begin(); iter != m_environmentListeners.end(); ++iter)
		{
			(*iter)->renderWindowResized(this);  
		}

    }




    void SceneManager::initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
        EnvironmentInfo::m_screenWidth = pBackBufferSurfaceDesc->Width;
        EnvironmentInfo::m_screenHeight = pBackBufferSurfaceDesc->Height;
        m_renderContext.setDevice(pd3dDevice);
    }

    void SceneManager::destroyWorld()
    {
        clearRenderQueues();
		m_renderGeometryManager.destroyAll();
		m_lightManager.destroyAll();

		for(unsigned int i = 0; i  < m_cameras.size(); ++i)
		{
			delete m_cameras[i];
		}
		m_cameras.clear();

    }

    void SceneManager::clearRenderQueues()
    {
        m_RenderBin.clearBins();
    }

    void SceneManager::update(double fTime, float fElapsedTime)
    {
        EnvironmentInfo::m_accumulatedTime = fTime;
		EnvironmentInfo::m_deltaTime = fElapsedTime;
       
    }


    void SceneManager::beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime)
    {

        m_renderContext.setDevice(pd3dDevice);
        m_renderContext.setImmediateContext(pd3dImmediateContext);
        

        
        // Clear render target and the depth stencil 
        float ClearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
        //pd3dImmediateContext->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), ClearColor );

        renderScene();

    }

    void SceneManager::renderScene()
    {
        
        m_renderContext.clearState();

		//iterate through camera
		
		std::sort(m_cameras.begin(), m_cameras.end(), [] ( RenderCamera* cam1, RenderCamera* cam2 ) -> bool
		{
			INT32 prio1 = cam1->getCameraPriority();
			INT32 prio2 = cam2->getCameraPriority();

			if(prio1 == prio2)
			{
				return cam1 < cam2;
			}

			return prio1 < prio2;
		});


		for( unsigned int i = 0; i < m_cameras.size(); ++i)
		{
			RenderCamera* cam = m_cameras[i];
			cullObjectsToRenderQueues(cam);
			cam->render(m_RenderBin, &m_renderContext);
				
		}

#if defined( DEBUG ) || defined( _DEBUG )
		drawDebug();
#endif
    }


	


    void SceneManager::cullObjectsToRenderQueues(RenderCamera* cam)
    {
		

        clearRenderQueues();

		cam->startedCulling();


		//add geometry
		std::map<RenderLayer, CullDataAllocator*>* cullDataPools = &m_renderGeometryManager.GetCullDataAllocators();
		cullObjectsFromPools(*cullDataPools, cam);

		//addlights
		cullDataPools = &m_lightManager.GetCullDataAllocators();
		cullObjectsFromPools(*cullDataPools, cam);
    }

	void SceneManager::cullObjectsFromPools(std::map<RenderLayer, CullDataAllocator*>& pools, RenderCamera* cam)
	{
		Frustrum frust;
		cam->calculateFrustrum(&frust);
		RenderLayer camMask = cam->getRenderMask();
		
		
		//cull all relevant pools
		for(auto iter = pools.begin(); iter != pools.end(); ++iter)
		{
			if(!(iter->first & camMask)) continue;

			CullDataAllocator* cullDataPool = iter->second;

			for( unsigned int i=0;i<cullDataPool->getNumberOfDynamicPoolVectors();++i)
			{

				CullDataPool &objects = cullDataPool->getDynamicPoolVector(i);
				m_culler->cull(frust,objects,m_cachedVisibleList);
			}

			

		}


		//construct renderbin
		for( UINT i = 0; i < m_cachedVisibleList.size(); ++i)
		{
			m_cachedVisibleList[i]->passedCulling(&m_RenderBin);
		}
		
		m_cachedVisibleList.clear();
	}
}