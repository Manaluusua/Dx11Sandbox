#include "SceneManager.h"
#include "Root.h"
#include "TextureManager.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "BasicRenderer.h"
#include "Frustrum.h"
#include "RenderObject.h"
#include "SIMDCuller.h"
namespace Dx11Sandbox
{
    SceneManager::SceneManager(Root* root)
        :m_root(root),
        m_renderer( new BasicRenderer() ),
        m_renderContext(),
        m_renderObjectListener(0),
        m_culler(new SIMDCuller()),
        m_screenWidth(0),
        m_screenHeight(0)
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
        SAFE_DELETE(m_culler);
    }

    void SceneManager::destroyManagers()
    {
        TextureManager::destroyTextureManager();
        MaterialManager::destroyMaterialManager();
        MeshManager::destroyMeshManager();
    }

    void SceneManager::addRenderStartListener(RenderStartListener* l)
    {
        m_renderStartListeners.insert(l);
    }

    void SceneManager::removeRenderStartListener(RenderStartListener* l)
    {
        m_renderStartListeners.erase(l);
    }


    void SceneManager::setRenderObjectListener(RenderObjectListener* l)
    {
        m_renderObjectListener = l;
    }



    void SceneManager::windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
    {
         // Setup the camera's projection parameters
        float aspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
        m_mainCamera.setProjection(D3DX_PI / 4, aspectRatio, 0.1f, 800.0f);

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

        renderScene( fTime,fElapsedTime, &m_mainCamera, m_renderer);
    }

    void SceneManager::renderScene( double fTime, float fElapsedTime, Camera* cam, Renderer* renderer)
    {
        
        m_renderContext.clearState();



        Frustrum frust;
        cam->calculateFrustrum(&frust);
        cullObjectsToRenderQueues(frust);

        //normal objects
        renderQueue( fTime, fElapsedTime,cam, RINITIAL,renderer);
        renderQueue(fTime, fElapsedTime,cam, RDEFAULT,renderer);
        renderQueue( fTime, fElapsedTime,cam, RTRANSPARENT,renderer);

        //objects using information from the previous objects rendered
        //TO DO
        renderQueue( fTime, fElapsedTime,cam, RSCENEINPUT,renderer);
        //Final
        renderQueue( fTime, fElapsedTime,cam,RFINAL,renderer);
    }


   void SceneManager::renderQueue( double fTime, float fElapsedTime,  Camera* cam,RenderQueueFlag flag, Renderer* renderer)
   {
        std::vector<const RenderObject*> & vec = m_renderqueues[flag];
        for(size_t i = 0;i<vec.size();++i)
        {
            if(m_renderObjectListener)
            {
                m_renderObjectListener->renderingObject(vec.at(i),&m_renderContext,this);
            }
            renderer->render(vec.at(i),&m_renderContext,cam);
        }
      
   }



    void SceneManager::cullObjectsToRenderQueues(Frustrum& frust)
    {

        clearRenderQueues();
        std::vector<const RenderObject*> nonCulled;
        //static scene
        for(int i=0;i<getNumberOfStaticPoolVectors();++i)
        {
            //cull
            const std::vector<RenderObject> &objects = *getStaticPoolVector(i);
            m_culler->cull(frust,objects,nonCulled);

            //send non-culled to rendering
            for(int j=0;j<nonCulled.size();++j)
            {
                const RenderObject* obj = nonCulled[j];
                m_renderqueues[obj->renderQueueFlag].push_back(obj);
            }
        }

        nonCulled.clear();
        //dynamic scene
        for(int i=0;i<getNumberOfDynamicPoolVectors();++i)
        {
             //cull
            const PoolVector<AllocationUnit<RenderObject> > &objects = getDynamicPoolVector(i);
            m_culler->cull(frust,objects,nonCulled);

            //send non-culled to rendering
            for(int j=0;j<nonCulled.size();++j)
            {
                const RenderObject* obj = nonCulled[j];
                m_renderqueues[obj->renderQueueFlag].push_back(obj);
            }
        }
    }
}