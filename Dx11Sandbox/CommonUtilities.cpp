#include "CommonUtilities.h"
#include "DXUT.h"
#include <sstream>
#include <functional>
#include <cstdlib>
namespace Dx11Sandbox
{
    void showErrorDialog(const WCHAR* msg)
    {
        MessageBox( DXUTGetHWND(), msg, L"DX11Sandbox", MB_ICONERROR | MB_OK );
    }
    void showErrorDialog(const char* msg)
    {
        WCHAR wstr[MAX_ERROR_LEN];
		multibyteStringToWide(msg, wstr, MAX_ERROR_LEN);
        showErrorDialog(wstr);

    }


	char* wideStringToMultibyte(wstring src)
	{
		int charCount = src.length() + 1;
		char* result = new char[charCount];

		wideStringToMultibyte(src.c_str(), result,charCount);
		return result;
	}

	WCHAR* multibyteStringToWide(string src)
	{
		int charCount = src.length() + 1;
		WCHAR* result = new WCHAR[charCount];
		multibyteStringToWide(src.c_str(),result, charCount);
		return result;
	}

	size_t wideStringToMultibyte(const WCHAR* src, char* dest, int count, bool makeNullTerminated)
	{
		int read = wcstombs(dest, src ,count);
		if(makeNullTerminated)
		{
			int lastIndex = min(read, count - 1);
			dest[lastIndex] = '\0';
		}
		return read;
	}

	size_t multibyteStringToWide(const char* src, WCHAR* dest, int count, bool makeNullTerminated)
	{	
		int read = mbstowcs(dest, src, count);

		if(makeNullTerminated)
		{
			int lastIndex = min(read, count - 1);
			dest[lastIndex] = '\0';
		}

		return read;
	}

	void printDebug(const char* text){
#if defined(DEBUG) | defined(_DEBUG)
		OutputDebugStringA(text);
#endif
	}

	void printDebug(const WCHAR* text){
#if defined(DEBUG) | defined(_DEBUG)
		OutputDebugStringW(text);
#endif
	}

	void printDebug(string text)
	{
#if defined(DEBUG) | defined(_DEBUG)

		OutputDebugStringA(text.c_str());
#endif
	}

    GIDTYPE generateID()
    {
       static GIDTYPE i = 0;
       ++i;
       return i;
    }

	ResourceID stringToID(const string& id)
	{
		return id;
		//return std::hash<std::string>()(id);
	}

    string numberToString(int number)
    {
        std::ostringstream stream;
        stream << number;
        return stream.str(); 
    }
}