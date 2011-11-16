
#include "Root.h"
#include "SceneManager.h"



Dx11Sandbox::Root* g_root = 0;

//global function hooks for DXUT
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                    DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return g_root->IsD3D11DeviceAcceptable(AdapterInfo ,Output,DeviceInfo, BackBufferFormat, bWindowed, pUserContext);
}



bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    return g_root->ModifyDeviceSettings(pDeviceSettings,pUserContext);
}



HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                    void* pUserContext )
{
    return g_root->OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc,pUserContext);
}


HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                        const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    return g_root->OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain,pBackBufferSurfaceDesc, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    g_root->OnFrameMove(fTime, fElapsedTime, pUserContext);
}



void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                double fTime, float fElapsedTime, void* pUserContext )
{
        
    g_root->OnD3D11FrameRender(pd3dDevice,pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
}



void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
    g_root->OnD3D11ReleasingSwapChain(pUserContext);
}

void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
    g_root->OnD3D11DestroyDevice(pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                            bool* pbNoFurtherProcessing, void* pUserContext )
{
    return g_root->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    g_root->OnKeyboard(nChar,bKeyDown, bAltDown, pUserContext);
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                        bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                        int xPos, int yPos, void* pUserContext )
{
    g_root->OnMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down, bSideButton2Down, nMouseWheelDelta,xPos, yPos, pUserContext);
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
    return g_root->OnDeviceRemoved(pUserContext);
}

namespace Dx11Sandbox
{
    Root* createRoot()
    {
        if(g_root)
        {
            return g_root;
        }
        // Set general DXUT callbacks
        DXUTSetCallbackFrameMove( OnFrameMove );
        DXUTSetCallbackKeyboard( OnKeyboard );
        DXUTSetCallbackMouse( OnMouse );
        DXUTSetCallbackMsgProc( MsgProc );
        DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
        DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );


        // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
        DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
        DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
        DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
        DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
        DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
        DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );

        if(!g_root)
        {
            g_root = new Root();
        }
        return g_root;

    }




    
    Root::Root()
        :m_mediaFolder(L"..\\Media\\"),
        m_application(0)
    {
        m_sceneMngr = Dx11Sandbox::SceneManager::createSceneManager(this);
    }

    Root::~Root()
    {
        if(m_application)
        {
            m_application->shutDown(m_sceneMngr);
        }
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
        m_sceneMngr->initialize(pd3dDevice, pBackBufferSurfaceDesc);
        m_application->createWorld(m_sceneMngr);
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
        m_application->update(m_sceneMngr, fTime,fElapsedTime);
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
        m_application->OnKeyboard(nChar, bKeyDown, bAltDown);
    }


    //--------------------------------------------------------------------------------------
    // Handle mouse button presses
    //--------------------------------------------------------------------------------------
    void Root::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                           bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                           int xPos, int yPos, void* pUserContext )
    {
        m_application->OnMouse(bLeftButtonDown,bRightButtonDown,bMiddleButtonDown,bSideButton1Down,bSideButton2Down,nMouseWheelDelta,xPos,yPos);
    }


    //--------------------------------------------------------------------------------------
    // Call if device was removed.  Return true to find a new device, false to quit
    //--------------------------------------------------------------------------------------
    bool Root::OnDeviceRemoved( void* pUserContext )
    {
        return true;
    }

    void Root::initialize(wstring windowName,UINT windowWidth,UINT windowHeight, D3D_FEATURE_LEVEL level, bool windowed)
    {
       
        if(!m_application)
        {
            showErrorDialog("no application set to root. Refusing to start...");
            return;
        }
        // Perform any application-level initialization here

        DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
        DXUTSetCursorSettings( false, false ); // Show the cursor and clip it when in full screen
        DXUTCreateWindow( windowName.c_str() );
        DXUTCreateDevice(level, windowed, windowWidth, windowHeight );
    }

    int Root::start()
    {
        if(!m_application)
        {
            showErrorDialog("no application set to root. Refusing to start...");
            return -1;
        }
        DXUTMainLoop();
        return DXUTGetExitCode();
    }
}

