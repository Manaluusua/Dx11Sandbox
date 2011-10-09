#include "SceneManager.h"
#include "Root.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "Mesh.h"
#include "BasicRenderer.h"
#include "RenderContext.h"
#include "RenderObject.h"

namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_renderObjectMask(0xFFFFFFFF),
        m_renderer( new BasicRenderer() ),
        m_renderContext(new RenderContext() )
    {
        m_renderqueues.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RINITIAL, new std::vector<RenderObject*>));
        m_renderqueues.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RDEFAULT, new std::vector<RenderObject*>));
        m_renderqueues.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RTRANSPARENT, new std::vector<RenderObject*>));
        m_renderqueues.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RSCENEINPUT, new std::vector<RenderObject*>));
        m_renderqueues.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RFINAL, new std::vector<RenderObject*>));
       
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
        SAFE_DELETE(m_renderqueues[RINITIAL]);
        SAFE_DELETE(m_renderqueues[RDEFAULT]);
        SAFE_DELETE(m_renderqueues[RTRANSPARENT]);
        SAFE_DELETE(m_renderqueues[RSCENEINPUT]);
        SAFE_DELETE(m_renderqueues[RFINAL]);
    }

    void SceneManager::destroyManagers()
    {
        TextureManager::destroyTextureManager();
        MaterialManager::destroyMaterialManager();
    }

    void SceneManager::addRenderObject(RenderObject* obj, RenderQueueFlag priority)
    {
        m_renderObjects.push_back(obj);
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



    void SceneManager::createWorld(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {

        //general scenemanager config
        setRenderObjectMask(0xF);

        //camera
        D3DXVECTOR3 vecEye( 0.0f, 20.0f, -2.0f );
        D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );
        m_mainCamera.SetViewParams( &vecEye, &vecAt );
        


        
        //objects
       /* Material* mat; 
        Mesh* mesh; 
        RenderObject *ro;

        //skybox
        mat = MaterialManager::getSingleton()->getOrCreateMaterial(pd3dDevice, L"skybox.fx", L"skybox",MeshInputLayouts::POS3TEX3);
        mesh = MeshUtility::createSkyBoxMesh(pd3dDevice);
        ro = new BasicRenderObject(mesh, mat);

        mat->setTexture("cubemap", L"skyboxCube.dds");
        TextureManager::getSingleton()->createTexture(pd3dDevice, L"skyboxCube.dds", L"skyboxCube.dds");
        
        addRenderObject(ro,Dx11Sandbox::RFINAL);
       
        //terrain
        mat = MaterialManager::getSingleton()->getOrCreateMaterial(pd3dDevice, L"terrain.fx", L"terrain1",MeshInputLayouts::POS3NORM3TEX2);
        mat->setTexture("texture1", L"grass.jpg");
        TextureManager::getSingleton()->createTexture(pd3dDevice, L"grass.jpg", L"grass.jpg");
        ro = MeshUtility::createTerrainFromHeightMap(pd3dDevice, L"heightmapTerrain.png", mat,500,500,150,30,30,20);
 
        addRenderObject(ro);
 */       

    }

    void SceneManager::destroyWorld()
    {
        for(int i=0;i<m_renderObjects.size();++i)
        {
            delete m_renderObjects[i];

        }
        m_renderObjects.clear();
    }

    void SceneManager::clearRenderQueues()
    {
        m_renderqueues[RINITIAL]->clear();
        m_renderqueues[RDEFAULT]->clear();
        m_renderqueues[RTRANSPARENT]->clear();
        m_renderqueues[RSCENEINPUT]->clear();
        m_renderqueues[RFINAL]->clear();
    }

    void SceneManager::update(double fTime, float fElapsedTime)
    {
        // Update the camera's position based on user input 
        m_mainCamera.FrameMove( fElapsedTime );
    }


    void SceneManager::beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime)
    {

        m_renderContext->setDevice(pd3dDevice);
        m_renderContext->setImmediateContext(pd3dImmediateContext);
        clearRenderQueues();

        //first notify all listeners and let them render what they need
        std::set<RenderListener*>::iterator it = m_renderListeners.begin();
        while(it != m_renderListeners.end())
        {
            (*it)->renderingStarted( m_renderContext,  fTime, fElapsedTime);
            ++it;
        }
        // Clear render target and the depth stencil 
        //float ClearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
        //pd3dImmediateContext->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), ClearColor );
        pd3dImmediateContext->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );
        renderScene( fTime,fElapsedTime);
    }

    void SceneManager::renderScene( double fTime, float fElapsedTime)
    {
        //normal objects
        renderQueue( fTime, fElapsedTime,&m_mainCamera, RINITIAL);
        renderQueue(fTime, fElapsedTime,&m_mainCamera, RDEFAULT);
        renderQueue( fTime, fElapsedTime,&m_mainCamera, RTRANSPARENT);

        //objects using information from the previous objects rendered
        //TO DO
        renderQueue( fTime, fElapsedTime,&m_mainCamera, RSCENEINPUT);
        //Final
        renderQueue( fTime, fElapsedTime,&m_mainCamera,RFINAL);
    }

    void SceneManager::handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext )
    {
        m_mainCamera.HandleMessages( hWnd, uMsg, wParam, lParam );
    }

   void SceneManager::renderQueue( double fTime, float fElapsedTime, const CBaseCamera* cam,RenderQueueFlag flag)
   {
        std::vector<RenderObject*> * vec = m_renderqueues[flag];
        UINT32 mask = m_renderObjectMask;
        for(unsigned int i=0;i<vec->size();i++)
        {
            if(mask & vec->at(i)->renderObjectMask)
                m_renderer->render(vec->at(i),m_renderContext,&m_mainCamera);
        }
   }
}