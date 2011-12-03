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
        float aspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
        m_mainCamera.setProjection(D3DX_PI / 4, aspectRatio, 0.1f, 1000.0f);
        
    }




    void SceneManager::initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {

        m_renderContext.setDevice(pd3dDevice);
    }

    void SceneManager::destroyWorld()
    {
        clearRenderQueues();
        deallocateStatic();
        deallocateDynamicAll();
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

    void SceneManager::renderScene( double fTime, float fElapsedTime, Camera* cam)
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

    //TO DO: the actual culling
    void SceneManager::cullObjectsToRenderQueues()
    {
        //static scene
        for(int i=0;i<getNumberOfStaticPoolVectors();i++)
        {
            const std::vector<RenderObject> *objects = getStaticPoolVector(i);
            for(int j=0;j<objects->size();++j)
            {
                const RenderObject& obj = objects->at(j);
                m_renderqueues[obj.renderQueueFlag].push_back(&obj);
            }
        }

        //dynamic scene
        for(int i=0;i<getNumberOfDynamicPoolVectors();i++)
        {
            const PoolVector<AllocationUnit<RenderObject> > &objects = getDynamicPoolVector(i);
            for(int j=0;j<objects.count;++j)
            {
                const RenderObject& obj = objects.vector.at(j).data;
                m_renderqueues[obj.renderQueueFlag].push_back(&obj);
            }
        }

    }


   void SceneManager::renderQueue( double fTime, float fElapsedTime,  Camera* cam,RenderQueueFlag flag)
   {
        std::vector<const RenderObject*> & vec = m_renderqueues[flag];
        m_renderer->render(vec,&m_renderContext,cam);
      
   }
}