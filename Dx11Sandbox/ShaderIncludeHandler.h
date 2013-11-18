#ifndef DX11SANDBOX_SHADERINCLUDEHANDLER_H
#define DX11SANDBOX_SHADERINCLUDEHANDLER_H

#include <D3Dcommon.h>

namespace Dx11Sandbox
{
	//a dumb include handler that can only search the files from the folder the includer is located at
	class ShaderIncludeHandler :public ID3DInclude
	{
	public:
		ShaderIncludeHandler(void);
		~ShaderIncludeHandler(void);

		virtual HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType,LPCSTR pFileName,LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
		virtual HRESULT __stdcall Close(LPCVOID pData);

	};

};
#endif
