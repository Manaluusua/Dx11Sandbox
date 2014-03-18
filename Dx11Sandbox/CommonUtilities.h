#ifndef DX11SANDBOX_COMMONUTILITIES_H
#define DX11SANDBOX_COMMONUTILITIES_H
#include "CommonTypes.h"



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

#ifndef CACHE_LINE
#define CACHE_LINE  32
#endif
#ifndef CACHE_ALIGN
#define CACHE_ALIGN __declspec(align(CACHE_LINE))
#endif


   
    
    //


    char* wideStringToMultibyte(const wstring src);
	WCHAR* multibyteStringToWide(const string src);

	size_t wideStringToMultibyte(const WCHAR* src, char* dest, int count, bool makeNullTerminated = true);
	size_t multibyteStringToWide(const char* src, WCHAR* dest, int count, bool makeNullTerminated = true);


	void printDebug(const char* text);
	void printDebug(const WCHAR* text);
	void printDebug(const string& text);

    void showErrorDialog(const WCHAR* msg);
    void showErrorDialog(const char* msg);

	ResourceID stringToID(const string& id);

    GIDTYPE generateID();

    string numberToString(int number);

}

#endif