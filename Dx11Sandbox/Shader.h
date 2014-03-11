#ifndef DX11SANDBOX_SHADER_H
#define DX11SANDBOX_SHADER_H

#include "ReleasePtr.h"
#include "RCObject.h"
#include "DXUT.h"
struct ID3DX11Effect;
struct ID3D11Device;

namespace Dx11Sandbox
{

	class Shader : public RCObject
	{
	public:

		Shader();
		Shader(const string& effectName, ID3D11Device* pd3dDevice);
		virtual ~Shader();

		Shader(const Shader& other);
		Shader& operator=(const Shader&);

		bool initializeEffect(const string& effectName, ID3D11Device* pd3dDevice);
		bool isShaderInitialized();
		ID3DX11Effect* getEffect();

	private:

		HRESULT CompileShaderFromFile(WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

		ReleasePtr<ID3DX11Effect> m_effect;


	};

};

#endif