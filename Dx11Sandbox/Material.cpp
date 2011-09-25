#include "Material.h"
#include <iostream>
namespace Dx11Sandbox
{

    Material::Material()
        :m_effect(0),
        m_layout(0)
    {
 

    }


    bool Material::loadAndInitializeMaterial(const wstring& effectName, ID3D11Device* pd3dDevice, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        bool succesfull = true;
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
        HRESULT hr;
        wstring errMsg;

#if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

        ID3DBlob* effectBuffer = 0;

        if(FAILED( CompileShaderFromFile( (WCHAR*)effectName.c_str(), 0,
                                    NULL, "fx_5_0", &effectBuffer ) ))
        {
            errMsg = L"Failed to compile effect file";
            errMsg += effectName.c_str();
            showErrorDialog( (WCHAR*)errMsg.c_str() );
            succesfull = false;
        }else
        {
            if(FAILED( D3DX11CreateEffectFromMemory( effectBuffer->GetBufferPointer(),
                                            effectBuffer->GetBufferSize(),
                                            0,
                                            pd3dDevice,
                                            &m_effect ) ))
            {
                errMsg = L"Failed to create effect file";
                errMsg += effectName.c_str();
                showErrorDialog( (WCHAR*)errMsg.c_str() );
                succesfull = false;
            }


            SAFE_RELEASE( effectBuffer );
        }
        
        if(!succesfull)
            return succesfull;


        D3DX11_PASS_DESC PassDesc;
        m_effect->GetTechniqueByIndex(0)->GetPassByIndex( 0 )->GetDesc( &PassDesc );
        hr =  pd3dDevice->CreateInputLayout( Dx11Sandbox::MeshInputLayouts::getElementDescForType(type),
            Dx11Sandbox::MeshInputLayouts::getElementCountForType(type), PassDesc.pIAInputSignature,
            PassDesc.IAInputSignatureSize, &m_layout );

        if(FAILED(hr))
            showErrorDialog("Failed to create input layout");

        return succesfull;
    }


    Material::~Material()
    {
        SAFE_RELEASE(m_effect);
        SAFE_RELEASE( m_layout );
    }

    void Material::setTexture(const string shaderVariable, const wstring textureName)
    {
        m_textureRefs[shaderVariable] = textureName;
    }





    //Copied from the directX SDK examples
    HRESULT Material::CompileShaderFromFile( WCHAR* szFileName, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
    {
        HRESULT hr = S_OK;

        // find the file
        WCHAR str[MAX_PATH];
        WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
        WCHAR *strLastSlash = NULL;
        bool  resetCurrentDir = false;

        // Get the current working directory so we can restore it later
        UINT nBytes = GetCurrentDirectory( MAX_PATH, workingPath );
        if (nBytes==MAX_PATH)
        {
          return E_FAIL;
        }

        // Check we can find the file first
        V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, szFileName ) );

        // Check if the file is in the current working directory
        wcscpy_s( filePath, MAX_PATH, str );

        strLastSlash = wcsrchr( filePath, TEXT( '\\' ) );
        if( strLastSlash )
        {
            // Chop the exe name from the exe path
            *strLastSlash = 0;

            SetCurrentDirectory( filePath );
            resetCurrentDir  = true;
        }

        // open the file
        HANDLE hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                   FILE_FLAG_SEQUENTIAL_SCAN, NULL );
        if( INVALID_HANDLE_VALUE == hFile )
            return E_FAIL;

        // Get the file size
        LARGE_INTEGER FileSize;
        GetFileSizeEx( hFile, &FileSize );

        // create enough space for the file data
        BYTE* pFileData = new BYTE[ FileSize.LowPart ];
        if( !pFileData )
            return E_OUTOFMEMORY;

        // read the data in
        DWORD BytesRead;
        if( !ReadFile( hFile, pFileData, FileSize.LowPart, &BytesRead, NULL ) )
            return E_FAIL; 

        CloseHandle( hFile );


        // Compile the shader using optional defines and an include handler for header processing
        ID3DBlob* pErrorBlob;
        hr = D3DCompile( pFileData, FileSize.LowPart, "none", 0,    0, 
                         szEntryPoint, szShaderModel, flags, D3DCOMPILE_EFFECT_ALLOW_SLOW_OPS, ppBlobOut, &pErrorBlob );

        delete []pFileData;

        // Restore the current working directory if we need to 
        if ( resetCurrentDir )
        {
            SetCurrentDirectory( workingPath );
        }


        if( FAILED(hr) )
        {
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
            SAFE_RELEASE( pErrorBlob );
            return hr;
        }
        SAFE_RELEASE( pErrorBlob );

        return hr;
    }

}