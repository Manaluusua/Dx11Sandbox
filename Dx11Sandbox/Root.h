#ifndef DX11SANDBOX_ROOT_H
#define DX11SANDBOX_ROOT_H

#include <DXUT.h>
#include "CommonUtilities.h"

namespace Dx11Sandbox
{
    class SceneManager;
    class Root
    {
    public:

        static Root* createRoot();
        virtual ~Root();

        const wstring& GetMediaPath() {return m_mediaFolder;}



        //callbacks from DXUT
        bool IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

        bool ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
        HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
                                      void* pUserContext );
        void OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
        void OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext );
        void OnD3D11ReleasingSwapChain( void* pUserContext );
        void OnD3D11DestroyDevice( void* pUserContext );
        LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
        void OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
        void OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                              bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                              int xPos, int yPos, void* pUserContext );
        bool OnDeviceRemoved( void* pUserContext );
        HRESULT OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
        //

    private:
        Root();
        SceneManager* m_sceneMngr;
        wstring m_mediaFolder;
    };
}
#endif