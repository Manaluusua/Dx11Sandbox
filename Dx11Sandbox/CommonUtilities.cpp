#include "CommonUtilities.h"
#include "DXUT.h"
#include <sstream>
namespace Dx11Sandbox
{
    void showErrorDialog(WCHAR* msg)
    {
        MessageBox( DXUTGetHWND(), msg, L"DX11Sandbox", MB_ICONERROR | MB_OK );
    }
    void showErrorDialog(char* msg)
    {
        WCHAR wstr[MAX_ERROR_LEN];
        MultiByteToWideChar( CP_ACP, 0, msg, -1, wstr, MAX_ERROR_LEN );
        showErrorDialog(wstr);

    }

    unsigned long generateID()
    {
       static unsigned long i = 0;
       ++i;
       return i;
    }
    string numberToString(int number)
    {
        std::ostringstream stream;
        stream << number;
        return stream.str(); 
    }
}