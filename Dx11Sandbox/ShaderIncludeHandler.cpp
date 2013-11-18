#include "ShaderIncludeHandler.h"
#include <string>
#include <fstream>

namespace Dx11Sandbox
{

	ShaderIncludeHandler::ShaderIncludeHandler(void)
	{
	}


	ShaderIncludeHandler::~ShaderIncludeHandler(void)
	{
	}

	HRESULT __stdcall ShaderIncludeHandler::Open(D3D_INCLUDE_TYPE IncludeType,LPCSTR pFileName,LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
	{
		 try {
			 
			 std::string path(pFileName);
			
			
			std::ifstream file(path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
 
			if (file.is_open()) {
				unsigned int fileSize = (unsigned int)file.tellg();
				char* buf = new char[fileSize];
				file.seekg (0, std::ios::beg);
				file.read (buf, fileSize);
				file.close();
				*ppData = buf;
				*pBytes = fileSize;
			} else {
				return E_FAIL;
			}
			return S_OK;
		}
		catch(std::exception&) {
			return E_FAIL;
		}
	}

	HRESULT __stdcall ShaderIncludeHandler::Close(LPCVOID pData)
	{
		char* buf = (char*)pData;
		delete[] buf;
		return S_OK;
	}
}