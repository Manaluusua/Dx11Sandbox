#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>
#include "Camera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "DynamicPoolAllocator.h"
#include "StaticPoolAllocator.h"

namespace Dx11Sandbox
{
    class RenderObject;
    class Material;
    class Renderer;
    class RenderContext;
    class SceneManager;
    
    // renderer listener to listen and act for rendering events
    class RenderStartListener
    {
    public:
        virtual void renderingStarted(RenderContext* context,SceneManager* mngr, double fTime, float fElapsedTime)=0;
        
    };

    class RenderObjectListener
    {
    public:
        virtual void renderingObject(const RenderObject* object, RenderContext* state,SceneManager* mngr)=0;
    };

    class SceneManager: public DynamicPoolAllocator<RenderObject>, public StaticPoolAllocator<RenderObject>
    {
    friend class Root;
    public:
        
       

        virtual ~SceneManager(void);

        inline void renderScene( double fTime, float fElapsedTime,  Camera* cam);
        inline void renderQueue( double fTime, float fElapsedTime,  Camera* cam,RenderQueueFlag flag);

        Camera& getMainCamera(){return m_mainCamera;};
        ID3D11Device* getDevice(){return m_renderContext.getDevice();}


        //listeners
        void addRenderStartListener(RenderStartListener* l);
        void removeRenderStartListener(RenderStartListener* l);

        void setRenderObjectListener(RenderObjectListener* l);


    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        
        void cullObjectsToRenderQueues();

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

        std::map<RenderQueueFlag, std::vector<const RenderObject*> > m_renderqueues;

        std::set<RenderStartListener*> m_renderStartListeners;

        //only 1 renderlistener can be assigned. Application itself can propagate the event to multiple recepients (with the possible killing of perf)
        RenderObjectListener* m_renderObjectListener;



        Root* m_root;
        Camera  m_mainCamera;
        

    private:
        static SceneManager* createSceneManager(Root* root);
        DISABLE_COPY(SceneManager)
        SceneManager(Root* root);
        Renderer* m_renderer;
        RenderContext m_renderContext;

    };
}
#endif
