#include "SceneManager.h"
#include "BasicRenderObject.h"
#include "Root.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "Mesh.h"
#include "CompositeObject.h"
namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_renderObjectMask(0xFFFFFFFF)
    {
       
    }

    SceneManager* SceneManager::createSceneManager(Root* root)
    {
        SceneManager* mngr = new SceneManager(root);
        mngr->m_renderObjects.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RINITIAL, new std::vector<RenderObject*>));
        mngr->m_renderObjects.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RDEFAULT, new std::vector<RenderObject*>));
        mngr->m_renderObjects.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RTRANSPARENT, new std::vector<RenderObject*>));
        mngr->m_renderObjects.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RSCENEINPUT, new std::vector<RenderObject*>));
        mngr->m_renderObjects.insert(std::pair<RenderQueueFlag,std::vector<RenderObject*>* >(RFINAL, new std::vector<RenderObject*>));
        return mngr;
    }


    SceneManager::~SceneManager(void)
    {
        destroyWorld();
        SAFE_DELETE(m_renderObjects[RINITIAL]);
        SAFE_DELETE(m_renderObjects[RDEFAULT]);
        SAFE_DELETE(m_renderObjects[RTRANSPARENT]);
        SAFE_DELETE(m_renderObjects[RSCENEINPUT]);
        SAFE_DELETE(m_renderObjects[RFINAL]);
        destroyManagers();
    }

    void SceneManager::destroyManagers()
    {
        TextureManager::destroyTextureManager();
        MaterialManager::destroyMaterialManager();
    }

    void SceneManager::addRenderObject(RenderObject* obj, RenderQueueFlag priority)
    {
        m_renderObjects[priority]->push_back(obj);
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
        Material* mat; 
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
        ro = MeshUtility::createTerrainFromHeightMap(pd3dDevice, L"heightmapTerrain.png", mat,500,500,150,20,20,20);
 
        addRenderObject(ro);
        

    }

    void SceneManager::destroyWorld()
    {
        clearRenderQueue(RINITIAL);
        clearRenderQueue(RDEFAULT);
        clearRenderQueue(RTRANSPARENT);
        clearRenderQueue(RSCENEINPUT);
        clearRenderQueue(RFINAL);
    }

    void SceneManager::clearRenderQueue(RenderQueueFlag flag)
    {
        std::vector<RenderObject*> * vec = m_renderObjects[flag];
        for(unsigned int i=0;i<vec->size();i++)
        {
            SAFE_DELETE(vec->at(i));
        }
        vec->clear();
    }

    void SceneManager::update(double fTime, float fElapsedTime)
    {
        // Update the camera's position based on user input 
        m_mainCamera.FrameMove( fElapsedTime );
    }


    void SceneManager::beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime)
    {
        //first notify all listeners and let them render what they need
        std::set<RenderListener*>::iterator it = m_renderListeners.begin();
        while(it != m_renderListeners.end())
        {
            (*it)->renderingStarted( pd3dDevice,  fTime, fElapsedTime);
            ++it;
        }
        // Clear render target and the depth stencil 
        //float ClearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
        //pd3dImmediateContext->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), ClearColor );
        pd3dImmediateContext->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0 );
        renderScene(pd3dDevice, pd3dImmediateContext, fTime,fElapsedTime);
    }

    void SceneManager::renderScene(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, double fTime, float fElapsedTime, Material* forcemat)
    {
        //normal objects
        renderQueue(pd3dDevice,context, fTime, fElapsedTime,&m_mainCamera,forcemat, RINITIAL);
        renderQueue(pd3dDevice,context, fTime, fElapsedTime,&m_mainCamera,forcemat, RDEFAULT);
        renderQueue(pd3dDevice,context, fTime, fElapsedTime,&m_mainCamera,forcemat, RTRANSPARENT);

        //objects using information from the previous objects rendered
        //TO DO
        renderQueue(pd3dDevice,context, fTime, fElapsedTime,&m_mainCamera,forcemat, RSCENEINPUT);
        //Final
        renderQueue(pd3dDevice,context, fTime, fElapsedTime,&m_mainCamera,forcemat, RFINAL);
    }

    void SceneManager::handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext )
    {
        m_mainCamera.HandleMessages( hWnd, uMsg, wParam, lParam );
    }

   void SceneManager::renderQueue(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, double fTime, float fElapsedTime, const CBaseCamera* cam, Material* forcemat,RenderQueueFlag flag)
   {
       std::vector<RenderObject*> * vec = m_renderObjects[flag];
        for(unsigned int i=0;i<vec->size();i++)
        {
            if(getRenderObjectMask() & vec->at(i)->getRenderObjectMask())
                vec->at(i)->render(pd3dDevice,context, cam,  fTime, fElapsedTime, forcemat);
        }
   }
}