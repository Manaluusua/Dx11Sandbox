#ifndef DX11SANDBOX_COMMONUTILITIES_H
#define DX11SANDBOX_COMMONUTILITIES_H

#include <windows.h>
#include <string>

struct ID3D11Device;
struct ID3DX11Mesh;

namespace Dx11Sandbox
{
#define MAX_ERROR_LEN 120

    typedef std::basic_string<TCHAR> wstring;
    typedef std::string string;

    void showErrorDialog(WCHAR* msg);
    void showErrorDialog(char* msg);
}

#endif