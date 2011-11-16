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
    
    
    // renderer listener to listen and act for rendering events
    class RenderListener
    {
    public:
        virtual void renderingStarted(RenderContext* context, double fTime, float fElapsedTime)=0;
        virtual void renderingQueue(RenderQueueFlag flag) = 0;
    };


    class SceneManager: public DynamicPoolAllocator<RenderObject>, public StaticPoolAllocator<RenderObject>
    {
    friend class Root;
    public:
        
       

        virtual ~SceneManager(void);
        void addRenderListener(RenderListener* l);
        void removeRenderListener(RenderListener* l);
        inline void renderScene( double fTime, float fElapsedTime,  Camera* cam);
        inline void renderQueue( double fTime, float fElapsedTime,  Camera* cam,RenderQueueFlag flag);

        Camera& getMainCamera(){return m_mainCamera;};
        ID3D11Device* getDevice(){return m_renderContext.getDevice();}

    protected:
        void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void initialize(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        void update(double fTime, float fElapsedTime);
        void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);

        void handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext );
        
        void cullObjectsToRenderQueues();

        void destroyWorld();
        void clearRenderQueues();
        void destroyManagers();

        std::map<RenderQueueFlag, std::vector<const RenderObject*> > m_renderqueues;
        std::set<RenderListener*> m_renderListeners;
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
