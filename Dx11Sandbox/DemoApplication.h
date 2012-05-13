#ifndef DX11SANDBOX_DEMOAPPLICATION_H
#define DX11SANDBOX_DEMOAPPLICATION_H

#include "MathUtil.h"
#include "Root.h"
#include "SceneManager.h"
using namespace Dx11Sandbox;
class WaterPlane;
class DemoApplication: public Application, public RenderObjectListener
{
public:

    DemoApplication();

    virtual void OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown);
    virtual  void OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                            bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                            int xPos, int yPos );
    virtual void createWorld(SceneManager* mngr);
    virtual void update(SceneManager* mngr,double fTime, float fElapsedTime);
    virtual void shutDown(SceneManager* mngr);
    virtual void handleWindowMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
    virtual void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);


    virtual void renderingObject(const RenderObject* object, RenderContext* state,SceneManager* mngr);

private:

    void handleInput(SceneManager* mngr,float dt, float elapsedTime);

    bool m_leftDown;
    bool m_rightDown;
    bool m_forwardDown;
    bool m_backwardDown;
    bool m_upDown;
    bool m_downDown;
    bool m_moveMouse;

    D3DXVECTOR2 m_lastMousePos;
    D3DXVECTOR2 m_mouseDelta;


    float m_time;

    //
    Material* m_lastMaterial;
    UINT16 m_lastPassID;
    WaterPlane* m_waterPlane;

};

#endif