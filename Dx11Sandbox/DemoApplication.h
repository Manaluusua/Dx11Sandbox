#ifndef DX11SANDBOX_DEMOAPPLICATION_H
#define DX11SANDBOX_DEMOAPPLICATION_H

#include "MathUtil.h"
#include "Root.h"
using namespace Dx11Sandbox;
class DemoApplication: public Application
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
};

#endif