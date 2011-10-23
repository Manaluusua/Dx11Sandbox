#include "SceneManager.h"
#include "Root.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "BasicRenderer.h"

#include "RenderObject.h"

namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_renderObjectMask(0xFFFFFFFF),
        m_renderer( new BasicRenderer() ),
        m_renderContext()
    {
       m_renderqueues[RINITIAL];
       m_renderqueues[RDEFAULT];
       m_renderqueues[RTRANSPARENT];
       m_renderqueues[RSCENEINPUT];
       m_renderqueues[RFINAL];

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
        m_renderqueues.clear();
        SAFE_DELETE(m_renderer);
    }

    void SceneManager::destroyManagers()
    {
        TextureManager::destroyTextureManager();
        MaterialManager::destroyMaterialManager();
        MeshManager::destroyMeshManager();
    }

    void SceneManager::addRenderListener(RenderListener* l)
    {
        m_renderListeners.insert(l);
    }

    void SceneManager::removeRenderListener(RenderListener* l)
    {
        m_renderListeners.erase(l);
    }

    void SceneManager::windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
         // Setup the camera's projection parameters
        float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
        m_mainCamera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 1000.0f );
        //m_mainCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
        
    }

    void SceneManager::handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext )
    {
        m_mainCamera.HandleMessages( hWnd, uMsg, wParam, lParam );
    }



    void SceneManager::createWorld(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {

        //general scenemanager config
        setRenderObjectMask(0xF);

        //camera
        D3DXVECTOR3 vecEye( 0.0f, 20.0f, -2.0f );
        D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
        m_mainCamera.SetViewParams( &vecEye, &vecAt );
        


        
        //objects
        Material* mat = 0; 
        Mesh* mesh = 0; 
        RenderObject *ro;

        //skybox
        mat = MaterialManager::getSingleton()->getOrCreateMaterial(pd3dDevice, L"skybox.fx", L"skybox",MeshInputLayouts::POS3TEX3);
        mesh = MeshUtility::createSkyBoxMesh(pd3dDevice, "skybox" + generateID());
        allocateRenderObject(&ro);
        ro->mat = mat;
        ro->mesh = mesh;
        ro->renderQueueFlag = RenderQueueFlag::RFINAL;

        mat->setTexture("cubemap", L"skyboxCube.dds");
        TextureManager::getSingleton()->createTexture(pd3dDevice, L"skyboxCube.dds", L"skyboxCube.dds");
     
        //terrain
        mat = MaterialManager::getSingleton()->getOrCreateMaterial(pd3dDevice, L"terrain.fx", L"terrain1",MeshInputLayouts::POS3NORM3TEX2);
        mat->setTexture("texture1", L"grass.jpg");
        TextureManager::getSingleton()->createTexture(pd3dDevice, L"grass.jpg", L"grass.jpg");
        MeshUtility::createTerrainFromHeightMap(pd3dDevice,this, L"heightmapTerrain.png", mat,500,500,50,30,30,20);

    }

    void SceneManager::destroyWorld()
    {
        clearRenderQueues();
        m_renderObjects.clear();
    }

    void SceneManager::clearRenderQueues()
    {
        m_renderqueues[RINITIAL].clear();
        m_renderqueues[RDEFAULT].clear();
        m_renderqueues[RTRANSPARENT].clear();
        m_renderqueues[RSCENEINPUT].clear();
        m_renderqueues[RFINAL].clear();
    }

    void SceneManager::update(double fTime, float fElapsedTime)
    {
        // Update the camera's position based on user input 
        m_mainCamera.FrameMove( fElapsedTime );
    }


    void SceneManager::beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime)
    {

        m_renderContext.setDevice(pd3dDevice);
        m_renderContext.setImmediateContext(pd3dImmediateContext);
        

        //first notify all listeners and let them render what they need
        std::set<RenderListener*>::iterator it = m_renderListeners.begin();
        while(it != m_renderListeners.end())
        {
            (*it)->renderingStarted( &m_renderContext,  fTime, fElapsedTime);
            ++it;
        }
        // Clear render target and the depth stencil 
        float ClearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
        pd3dImmediateContext->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), ClearColor );
        pd3dImmediateContext->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );
        renderScene( fTime,fElapsedTime, &m_mainCamera);
    }

    void SceneManager::renderScene( double fTime, float fElapsedTime,const CBaseCamera* cam)
    {
        clearRenderQueues();
        m_renderContext.clearState();
        cullObjectsToRenderQueues();

        //normal objects
        renderQueue( fTime, fElapsedTime,cam, RINITIAL);
        renderQueue(fTime, fElapsedTime,cam, RDEFAULT);
        renderQueue( fTime, fElapsedTime,cam, RTRANSPARENT);

        //objects using information from the previous objects rendered
        //TO DO
        renderQueue( fTime, fElapsedTime,cam, RSCENEINPUT);
        //Final
        renderQueue( fTime, fElapsedTime,cam,RFINAL);
    }

    bool SceneManager::allocateRenderObjects(unsigned int count, RenderObject** objectPointers, bool adjacent)
    {
        m_renderObjects.resize(m_renderObjects.size() +count);
        if(!adjacent)
            return false;
        *objectPointers = &m_renderObjects.back() - (count-1);
        return true;
    }
    bool SceneManager::allocateRenderObject( RenderObject** objectPointer)
    {
        m_renderObjects.resize(m_renderObjects.size() +1);
        *objectPointer = &m_renderObjects.back();
        return true;
    }


    void SceneManager::cullObjectsToRenderQueues()
    {
        for(int i=0;i<m_renderObjects.size();i++)
        {
            //TO DO: culling
            RenderObject& obj = m_renderObjects.at(i);
            m_renderqueues[obj.renderQueueFlag].push_back(&obj);
        }
    }


   void SceneManager::renderQueue( double fTime, float fElapsedTime, const CBaseCamera* cam,RenderQueueFlag flag)
   {
        std::vector<RenderObject*> & vec = m_renderqueues[flag];
        UINT32 mask = m_renderObjectMask;
        for(unsigned int i=0;i<vec.size();i++)
        {
            if(mask & vec.at(i)->renderObjectMask)
                m_renderer->render(vec.at(i),&m_renderContext,&m_mainCamera);
        }
   }
}