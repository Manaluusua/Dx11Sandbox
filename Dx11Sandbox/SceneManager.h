#ifndef DX11SANDBOX_SCENEMANAGER_H
#define DX11SANDBOX_SCENEMANAGER_H

#include "DXUT.h"
#include <map>
#include <set>
#include <vector>
#include "DXUTcamera.h"

namespace Dx11Sandbox
{
    
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
        virtual void renderingStarted(ID3D11Device* pd3dDevice, double fTime, float fElapsedTime)=0;
    };

    class RenderObject;
    class Material;

    class SceneManager
    {
    friend class Root;
    public:
        
        static SceneManager* createSceneManager(Root* root);

        virtual ~SceneManager(void);
        virtual void addRenderObject(RenderObject* obj, RenderQueueFlag priority = RDEFAULT);
        virtual void addRenderListener(RenderListener* l);
        virtual void removeRenderListener(RenderListener* l);
        virtual void renderScene(ID3D11Device* pd3dDevice, double fTime, float fElapsedTime, Material* forcemat=0);
        virtual void renderQueue(ID3D11Device* pd3dDevice, double fTime, float fElapsedTime, Material* forcemat,RenderQueueFlag flag);

        virtual CBaseCamera& getMainCamera(){return m_mainCamera;};

    protected:
        virtual void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        virtual void createWorld(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
        virtual void update(double fTime, float fElapsedTime);
        virtual void beginDraw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime);

        virtual void handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext );
       
        virtual void destroyWorld();
        virtual void clearRenderQueue(RenderQueueFlag flag);
        virtual void destroyManagers();

        std::map<RenderQueueFlag, std::vector<RenderObject*>* > m_renderObjects;
        std::set<RenderListener*> m_renderListeners;
        Root* m_root;
        CModelViewerCamera  m_mainCamera;

    private:
        SceneManager(Root* root);

    };
}
#endif
