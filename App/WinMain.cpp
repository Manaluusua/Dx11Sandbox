
#include "Root.h"
#include "DemoApplication.h"



 
//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
	
    Dx11Sandbox::Root* root = Dx11Sandbox::createRoot();
    root->setApplication( new DemoApplication );
	root->initialize("Dx11SandBox", 1024, 768,D3D_FEATURE_LEVEL_11_0 , true);

    int exitcode = root->start();
    
    SAFE_DELETE(root);
	
    return exitcode;
}