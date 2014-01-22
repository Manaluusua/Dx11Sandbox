#include "Material.h"
#include "RenderContext.h"
#include "Texture.h"
#include "TextureManager.h"
#include "ShaderIncludeHandler.h"
#include <iostream>


namespace Dx11Sandbox
{
    Material::Material()
        :m_effect(0),
        m_layout(0)
    {
 

    }


	Material::Material(const Material& other)
		:m_effect(other.m_effect),
		m_layout(other.m_layout),
		m_textureRefs(other.m_textureRefs)
	{
	}
	
	Material& Material::operator=(const Material& other)
	{
		if(&other == this) return *this;
		m_effect = other.m_effect;
		m_layout = other.m_layout;
		m_textureRefs = other.m_textureRefs;
		
		return *this;
	}

    bool Material::loadAndInitializeMaterial(const string& effectName, ID3D11Device* pd3dDevice, Dx11Sandbox::MeshInputLayouts::MESH_LAYOUT_TYPE type)
    {
        bool succesfull = true;
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
        HRESULT hr;
        string errMsg;

		ID3DX11Effect* effect;
        ID3D11InputLayout* layout;

		std::unique_ptr<WCHAR> effectNameWide( MultibyteStringToWide(effectName) );

#if defined( DEBUG ) || defined( _DEBUG )
        dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

        ID3DBlob* effectBuffer = 0;

		if(FAILED( CompileShaderFromFile( effectNameWide.get(), 0,
                                    NULL, "fx_5_0", &effectBuffer ) ))
        {
            errMsg = "Failed to compile effect file";
            errMsg += effectName.c_str();
            showErrorDialog( errMsg.c_str() );
            succesfull = false;
        }else
        {
            if(FAILED( D3DX11CreateEffectFromMemory( effectBuffer->GetBufferPointer(),
                                            effectBuffer->GetBufferSize(),
                                            0,
                                            pd3dDevice,
                                            &effect ) ))
            {
                errMsg = "Failed to create effect file";
                errMsg += effectName.c_str();
                showErrorDialog( errMsg.c_str() );
                succesfull = false;
            }
			m_effect = effect;
			effect->Release();

            SAFE_RELEASE( effectBuffer );
        }
        
        if(!succesfull)
            return succesfull;


        D3DX11_PASS_DESC PassDesc;
        m_effect->GetTechniqueByIndex(0)->GetPassByIndex( 0 )->GetDesc( &PassDesc );
        hr =  pd3dDevice->CreateInputLayout( Dx11Sandbox::MeshInputLayouts::getElementDescForType(type),
            Dx11Sandbox::MeshInputLayouts::getElementCountForType(type), PassDesc.pIAInputSignature,
            PassDesc.IAInputSignatureSize, &layout );

		
        if(FAILED(hr))
            showErrorDialog("Failed to create input layout");

		m_layout = layout;
		layout->Release();

        return succesfull;
    }


    Material::~Material()
    {
        
        
    }

    void Material::setTexture(const string& shaderVariable, ResourceID textureID)
    {
        m_textureRefs[shaderVariable] = textureID;
    }

    bool Material::bind(RenderContext* context)
    {
        
        if( this == context->getBoundMaterial() ) 
        {
            return false;
        }
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;
        
        
        m_effect->GetDesc(&effectDesc);
        
        //for now use first tech
        ID3DX11EffectTechnique* tech = m_effect->GetTechniqueByIndex(0);
        tech->GetDesc(&techDesc);

        tech->GetPassByIndex(0)->GetDesc(&passDesc);
        
        context->getImmediateContext()->IASetInputLayout( m_layout );

  
        for(std::map<string, ResourceID>::iterator iter = m_textureRefs.begin(); iter!=m_textureRefs.end(); ++iter)
        {
           
            ID3DX11EffectVariable* var = m_effect->GetVariableByName(iter->first.c_str());
            if(var->IsValid())
            {
                ID3DX11EffectShaderResourceVariable* texResource = var->AsShaderResource();
                Texture* tex = TextureManager::singleton()->getTexture(iter->second);
                if(tex)
                {
                    texResource->SetResource(tex->GetShaderResourceView());
                }
            }
        }

        return true;
    }



    //Copied from the directX SDK examples
    HRESULT Material::CompileShaderFromFile( WCHAR* pathToShader, DWORD flags, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
    {
        HRESULT hr = S_OK;

        WCHAR workingPath[MAX_PATH], filePath[MAX_PATH];
        WCHAR *strLastSlash = NULL;
        bool  resetCurrentDir = false;
		ShaderIncludeHandler includeHandler;

        // Get the current working directory so we can restore it later
        UINT nBytes = GetCurrentDirectory( MAX_PATH, workingPath );
        if (nBytes==MAX_PATH)
        {
          return E_FAIL;
        }


        // Check if the file is in the current working directory
        wcscpy_s( filePath, MAX_PATH, pathToShader );

        strLastSlash = wcsrchr( filePath, TEXT( '\\' ) );
        if( strLastSlash )
        {
            // Chop the exe name from the exe path
            *strLastSlash = 0;

            SetCurrentDirectory( filePath );
            resetCurrentDir  = true;
        }

        // open the file
        HANDLE hFile = CreateFile( pathToShader, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
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
		hr = D3DCompile( pFileData, FileSize.LowPart, "none", 0,   &includeHandler, 
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