
#include "Root.h"
    
    //gobal root variable
    Dx11Sandbox::Root *root;



    bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
    {
        return root->IsD3D11DeviceAcceptable(AdapterInfo ,Output,DeviceInfo, BackBufferFormat, bWindowed, pUserContext);
    }



    bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
    {
        return root->ModifyDeviceSettings(pDeviceSettings,pUserContext);
    }



    HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext )
    {
        return root->OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc,pUserContext);
    }


    HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
    {
        return root->OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain,pBackBufferSurfaceDesc, pUserContext);
    }


    //--------------------------------------------------------------------------------------
    // Handle updates to the scene.  This is called regardless of which D3D API is used
    //--------------------------------------------------------------------------------------
    void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
    {
        root->OnFrameMove(fTime, fElapsedTime, pUserContext);
    }



    void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
    {
        root->OnD3D11FrameRender(pd3dDevice,pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
    }



    void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
    {
        root->OnD3D11ReleasingSwapChain(pUserContext);
    }

    void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
    {
        root->OnD3D11DestroyDevice(pUserContext);
    }


    //--------------------------------------------------------------------------------------
    // Handle messages to the application
    //--------------------------------------------------------------------------------------
    LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                              bool* pbNoFurtherProcessing, void* pUserContext )
    {
        return root->MsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
    }


    //--------------------------------------------------------------------------------------
    // Handle key presses
    //--------------------------------------------------------------------------------------
    void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
    {
        root->OnKeyboard(nChar,bKeyDown, bAltDown, pUserContext);
    }


    //--------------------------------------------------------------------------------------
    // Handle mouse button presses
    //--------------------------------------------------------------------------------------
    void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                           bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                           int xPos, int yPos, void* pUserContext )
    {
        root->OnMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down, bSideButton2Down, nMouseWheelDelta,xPos, yPos, pUserContext);
    }


    //--------------------------------------------------------------------------------------
    // Call if device was removed.  Return true to find a new device, false to quit
    //--------------------------------------------------------------------------------------
    bool CALLBACK OnDeviceRemoved( void* pUserContext )
    {
        return root->OnDeviceRemoved(pUserContext);
    }
//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    root = Dx11Sandbox::Root::createRoot();

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

    // Perform any application-level initialization here

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Dx11Sandbox" );
    DXUTCreateDevice(D3D_FEATURE_LEVEL_10_1, true, 640, 480 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    // Perform any application-level cleanup here
    int exitcode = DXUTGetExitCode();
    SAFE_DELETE(root);
    return exitcode;
}