#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>
#include "Camera.h"
#include "CommonUtilities.h"
#include "RenderContext.h"
#include "CullInfo.h"
#include "DynamicPoolAllocator.h"
#include "StaticPoolAllocator.h"

namespace Dx11Sandbox
{
    class CullInfo;
    class Material;
    class Renderer;
    class RenderContext;
    class SceneManager;
    class Frustrum;
    class Culler;

    
    
    // renderer listener to listen and act for rendering events
    class RenderStartListener
    {
    public:
        virtual void renderingStarted(RenderContext* context,SceneManager* mngr, double fTime, float fElapsedTime)=0;
        
    };

    class RenderObjectListener
    {
    public:
        virtual void renderingObject(const CullInfo* object, RenderContext* state,SceneManager* mngr)=0;
    };

    class SceneManager: public DynamicPoolAllocator<CullInfo>
    {
    friend class Root;
    public:
        
       

        virtual ~SceneManager(void);

        inline void renderScene( double fTime, float fElapsedTime,  Camera* cam, Renderer* renderer);
        inline void renderQueue( double fTime, float fElapsedTime,  Camera* cam,RenderQueueFlag flag, Renderer* renderer);

        Camera& getMainCamera(){return m_mainCamera;};
        RenderContext* getRenderContext();

        Renderer* getDefaultRenderer(){return m_renderer;};

        //listeners
        void addRenderStartListener(RenderStartListener* l);
        void removeRenderStartListener(RenderStartListener* l);

        void setRenderObjectListener(RenderObjectListener* l);

        UINT getScreenWidth(){return m_screenWidth;}
        UINT getScreenHeight(){return m_screenHeight;}


         void cullObjectsToRenderQueues(Frustrum& frust);

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);
        
       

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

        std::map<RenderQueueFlag, std::vector<const CullInfo*> > m_renderqueues;

        std::set<RenderStartListener*> m_renderStartListeners;

        //only 1 renderlistener can be assigned. Application itself can propagate the event to multiple recepients (with the possible killing of perf)
        RenderObjectListener* m_renderObjectListener;

        Culler* m_culler;

        Root* m_root;
        Camera  m_mainCamera;

        UINT m_screenWidth, m_screenHeight;
        

    private:
        static SceneManager* createSceneManager(Root* root);
        DISABLE_COPY(SceneManager)
        SceneManager(Root* root);
        Renderer* m_renderer;
        RenderContext m_renderContext;

        std::vector<const CullInfo*> m_cachedVisibleList;


    };


    inline  RenderContext* SceneManager::getRenderContext()
    {
        return &m_renderContext;
    }
}


#endif
