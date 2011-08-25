
#include "Root.h"
#include "SceneManager.h"

namespace Dx11Sandbox
{


    Root::Root()
        :m_mediaFolder(L"..\\Media\\")
    {
    }

    Root* Root::createRoot()
    {
        Root* root = new Root();
        root->m_sceneMngr = Dx11Sandbox::SceneManager::createSceneManager(root);
        return root;
    }
    Root::~Root()
    {
        SAFE_DELETE(m_sceneMngr);
    }

    
    //--------------------------------------------------------------------------------------
    // Reject any D3D11 devices that aren't acceptable by returning false
    //-- ------------------------------------------------------------------------------------
    bool Root::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
    {
        return true;
    }


    //--------------------------------------------------------------------------------------
    // Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
    //--------------------------------------------------------------------------------------
    bool Root::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
    {
        return true;
    }


    //--------------------------------------------------------------------------------------
    // Create any D3D11 resources that aren't dependant on the back buffer
    //--------------------------------------------------------------------------------------
    HRESULT Root::OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
    {
        m_sceneMngr->createWorld(pd3dDevice, pBackBufferSurfaceDesc);
        return S_OK;
    }


    //--------------------------------------------------------------------------------------
    // Create any D3D11 resources that depend on the back buffer
    //--------------------------------------------------------------------------------------
    HRESULT Root::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
    {
        m_sceneMngr->windowResized(pd3dDevice,pSwapChain,pBackBufferSurfaceDesc);
        return S_OK;
    }


    //--------------------------------------------------------------------------------------
    // Handle updates to the scene.  This is called regardless of which D3D API is used
    //--------------------------------------------------------------------------------------
    void Root::OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
    {
        m_sceneMngr->update(fTime, fElapsedTime);
    }


    //--------------------------------------------------------------------------------------
    // Render the scene using the D3D11 device
    //--------------------------------------------------------------------------------------
    void Root::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
    {
        m_sceneMngr->beginDraw(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime);
    }


    //--------------------------------------------------------------------------------------
    // Release D3D11 resources created in OnD3D11ResizedSwapChain 
    //--------------------------------------------------------------------------------------
    void Root::OnD3D11ReleasingSwapChain( void* pUserContext )
    {
    }


    //--------------------------------------------------------------------------------------
    // Release D3D11 resources created in OnD3D11CreateDevice 
    //--------------------------------------------------------------------------------------
    void Root::OnD3D11DestroyDevice( void* pUserContext )
    {
    }


    //--------------------------------------------------------------------------------------
    // Handle messages to the application
    //--------------------------------------------------------------------------------------
    LRESULT Root::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext )
    {
        m_sceneMngr->handleWindowMessages(hWnd, uMsg, wParam, lParam,pbNoFurtherProcessing,pUserContext);
        return 0;
    }


    //--------------------------------------------------------------------------------------
    // Handle key presses
    //--------------------------------------------------------------------------------------
    void Root::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
    {
    }


    //--------------------------------------------------------------------------------------
    // Handle mouse button presses
    //--------------------------------------------------------------------------------------
    void Root::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                           bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                           int xPos, int yPos, void* pUserContext )
    {
    }


    //--------------------------------------------------------------------------------------
    // Call if device was removed.  Return true to find a new device, false to quit
    //--------------------------------------------------------------------------------------
    bool Root::OnDeviceRemoved( void* pUserContext )
    {
        return true;
    }




}

