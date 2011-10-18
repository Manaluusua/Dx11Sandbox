#ifndef DX11SANDBOX_COMMONUTILITIES_H
#define DX11SANDBOX_COMMONUTILITIES_H

#include <windows.h>
#include <string>

namespace Dx11Sandbox
{
#define MAX_ERROR_LEN 120

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


#ifndef DISABLE_COPY
#define DISABLE_COPY(T)     T(const T&); T& operator=(const T&);
#endif
    enum RenderQueueFlag{
        RINITIAL, //first objects, background?
        RDEFAULT, //opaque
        RTRANSPARENT, //transparent
        RSCENEINPUT, //here the previous scene is available for input
        RFINAL //last
    };

    const double PI = 3.14159265;

    typedef std::basic_string<TCHAR> wstring;
    typedef std::string string;

    void showErrorDialog(WCHAR* msg);
    void showErrorDialog(char* msg);

    unsigned long generateID();

    string numberToString(int number);

}

#endif