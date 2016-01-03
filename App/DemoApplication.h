#ifndef DX11SANDBOX_DEMOAPPLICATION_H
#define DX11SANDBOX_DEMOAPPLICATION_H

#include "MathUtil.h"
#include "Root.h"
#include "SceneManager.h"
#include "RenderBin.h"
#include "RCObjectPtr.h"
#include "WaterPlane.h"


using namespace Dx11Sandbox;
class WaterPlane;
namespace Dx11Sandbox
{
	class DebugDrawTextureToScreen;
	class DebugDrawLights;
};


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
    virtual void windowResized(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);


    virtual void objectBeingRendered(CullableGeometry* obj);

private:

	void createMaterialBalls(SceneManager* mngr);
    void handleInput(SceneManager* mngr,float dt, float elapsedTime);

    bool m_leftDown;
    bool m_rightDown;
    bool m_forwardDown;
    bool m_backwardDown;
    bool m_upDown;
    bool m_downDown;
    bool m_moveMouse;

	DebugDrawTextureToScreen* m_debugDrawerTexture;
	DebugDrawLights* m_debugDrawerLights;

    Vec2 m_lastMousePos;
    Vec2 m_mouseDelta;

	float m_mouseSensitivity;

    float m_time;

    //ss
    RCObjectPtr<WaterPlane> m_waterPlane;
    SceneManager* m_mngr;
	Light* sun;
};

#endif