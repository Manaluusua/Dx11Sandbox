#include "Shader.h"
#include "ShaderIncludeHandler.h"

#include "SDKmisc.h"

#include <d3dx11effect.h>
#include <memory>
namespace Dx11Sandbox
{

	Shader::Shader()
	{

	}

	Shader::Shader(const string& effectName, ID3D11Device* pd3dDevice)
	{
		initializeEffect(effectName, pd3dDevice);
	}


	Shader::~Shader()
	{
	}

	bool Shader::isShaderInitialized()
	{
		return m_effect.rawPtr() != 0;
	}

	Shader::Shader(const Shader& other)
		:m_effect(other.m_effect)
	{

	}
	Shader& Shader::operator=(const Shader& other)
	{
		if (&other == this) return *this;
		m_effect = other.m_effect;
		
		return *this;
	}
	
	ID3DX11Effect* Shader::getEffect(){
		return m_effect;
	}

	bool Shader::initializeEffect(const string& effectName, ID3D11Device* pd3dDevice)
	{
		bool succesfull = true;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		HRESULT hr;
		string errMsg;

		ID3DX11Effect* effect;
		ID3D11InputLayout* layout;

		std::unique_ptr<WCHAR> effectNameWide(multibyteStringToWide(effectName));

#if defined( DEBUG ) || defined( _DEBUG )
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* effectBuffer = 0;

		if (FAILED(CompileShaderFromFile(effectNameWide.get(), 0,
			NULL, "fx_5_0", &effectBuffer)))
		{
			errMsg = "Failed to compile effect file";
			errMsg += effectName.c_str();
			showErrorDialog(errMsg.c_str());
			succesfull = false;
		}
		else
		{
			if (FAILED(D3DX11CreateEffectFromMemory(effectBuffer->GetBufferPointer(),
				effectBuffer->GetBufferSize(),
				0,
				pd3dDevice,
				&effect)))
			{
				errMsg = "Failed to create effect file";
				errMsg += effectName.c_str();
				showErrorDialog(errMsg.c_str());
				succesfull = false;
			}
			m_effect = effect;
			effect->Release();

			SAFE_RELEASE(effectBuffer);
		}

		return succesfull;
	}

	//Copied from the directX SDK examples
	HRESULT Shader::CompileShaderFromFile(WCHAR* pathToShader, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
		WCHAR *strLastSlash = NULL;
		bool  resetCurrentDir = false;
		ShaderIncludeHandler includeHandler;

		// Get the current working directory so we can restore it later
		UINT nBytes = GetCurrentDirectory(MAX_PATH, workingPath);
		if (nBytes == MAX_PATH)
		{
			return E_FAIL;
		}


		// Check if the file is in the current working directory
		wcscpy_s(filePath, MAX_PATH, pathToShader);

		strLastSlash = wcsrchr(filePath, TEXT('\\'));
		if (strLastSlash)
		{
			// Chop the exe name from the exe path
			*strLastSlash = 0;

			SetCurrentDirectory(filePath);
			resetCurrentDir = true;
		}

		// open the file
		HANDLE hFile = CreateFile(pathToShader, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
			return E_FAIL;

		// Get the file size
		LARGE_INTEGER FileSize;
		GetFileSizeEx(hFile, &FileSize);

		// create enough space for the file data
		BYTE* pFileData = new BYTE[FileSize.LowPart];
		if (!pFileData)
			return E_OUTOFMEMORY;

		// read the data in
		DWORD BytesRead;
		if (!ReadFile(hFile, pFileData, FileSize.LowPart, &BytesRead, NULL))
			return E_FAIL;

		CloseHandle(hFile);


		// Compile the shader using optional defines and an include handler for header processing
		ID3DBlob* pErrorBlob;
		hr = D3DCompile(pFileData, FileSize.LowPart, "none", 0, &includeHandler,
			szEntryPoint, szShaderModel, flags, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, ppBlobOut, &pErrorBlob);

		delete[]pFileData;

		// Restore the current working directory if we need to 
		if (resetCurrentDir)
		{
			SetCurrentDirectory(workingPath);
		}


		if (FAILED(hr))
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			SAFE_RELEASE(pErrorBlob);
			return hr;
		}
		SAFE_RELEASE(pErrorBlob);

		return hr;
	}


}