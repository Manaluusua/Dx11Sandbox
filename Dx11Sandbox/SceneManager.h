#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>
#include "DXUTcamera.h"

namespace Dx11Sandbox
{
    class RenderObject;
    class Material;
    class Renderer;
    class RenderContext;
    
    enum RenderQueueFlag{
        RINITIAL, //first objects, background?
        RDEFAULT, //opaque
        RTRANSPARENT, //transparent
        RSCENEINPUT, //here the previous scene is available for input
        RFINAL //last
    };

    
    // renderer listener to listen and act for rendering events
    class RenderListener
    {
    public:
        virtual void renderingStarted(RenderContext* context, double fTime, float fElapsedTime)=0;
        virtual void renderingQueue(RenderQueueFlag flag) = 0;
    };


    class SceneManager
    {
    friend class Root;
    public:
        
        static SceneManager* createSceneManager(Root* root);

        virtual ~SceneManager(void);
        void addRenderObject(RenderObject* obj, RenderQueueFlag priority = RDEFAULT);
        void addRenderListener(RenderListener* l);
        void removeRenderListener(RenderListener* l);
        inline void renderScene( double fTime, float fElapsedTime);
        inline void renderQueue( double fTime, float fElapsedTime, const CBaseCamera* cam,RenderQueueFlag flag);

        CBaseCamera& getMainCamera(){return m_mainCamera;};

        UINT32 getRenderObjectMask(){return m_renderObjectMask;}
        void setRenderObjectMask(UINT32 mask){m_renderObjectMask=mask;}

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void createWorld(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);

        void handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext );
       
        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

        std::vector<RenderObject*> m_renderObjects;
        std::map<RenderQueueFlag, std::vector<RenderObject*>* > m_renderqueues;
        std::set<RenderListener*> m_renderListeners;
        Root* m_root;
        CFirstPersonCamera  m_mainCamera;
        UINT32 m_renderObjectMask;

    private:
        SceneManager(Root* root);
        Renderer* m_renderer;
        RenderContext* m_renderContext;

    };
}
#endif
