#include "Material.h"
#include "RenderContext.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Shader.h"
#include "InputLayoutDescription.h"
#include <iostream>
#include <memory>
#include <D3D11.h>
#include <d3dx11effect.h>
namespace Dx11Sandbox
{
    Material::Material()
        :m_shader(0),
        m_layout(0)
    {
 

    }


	Material::Material(const Material& other)
		:m_shader(other.m_shader),
		m_layout(other.m_layout),
		m_textureRefs(other.m_textureRefs)
	{
	}
	
	Material& Material::operator=(const Material& other)
	{
		if(&other == this) return *this;
		m_shader = other.m_shader;
		m_layout = other.m_layout;
		m_textureRefs = other.m_textureRefs;
		
		return *this;
	}

	Shader* Material::getShader()
	{
		return m_shader;
	}
	ID3D11InputLayout* Material::getInputLayout()
	{
		return m_layout;
	}

	std::map<string, ResourceID>& Material::getTextureReferences()
	{
		return m_textureRefs;
	}

	bool Material::loadAndInitializeMaterial(const string& effectName, ID3D11Device* pd3dDevice, const InputLayoutDescription& inputDescription)
    {
        bool succesfull = true;
		ID3D11InputLayout* layout;
		m_shader = new Shader(effectName, pd3dDevice);
		succesfull = m_shader->isShaderInitialized();
		if (!succesfull){
			return succesfull;
		}

        D3DX11_PASS_DESC PassDesc;
        m_shader->getEffect()->GetTechniqueByIndex(0)->GetPassByIndex( 0 )->GetDesc( &PassDesc );
		HRESULT hr = pd3dDevice->CreateInputLayout(inputDescription.getElementDescriptions(),
			inputDescription.getElementCount(), PassDesc.pIAInputSignature,
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
        
		ID3DX11Effect* effect = m_shader->getEffect();

        
        
        context->getImmediateContext()->IASetInputLayout( m_layout );

  
        for(std::map<string, ResourceID>::iterator iter = m_textureRefs.begin(); iter!=m_textureRefs.end(); ++iter)
        {
           
			ID3DX11EffectVariable* var = effect->GetVariableByName(iter->first.c_str());
            if(var->IsValid())
            {
                ID3DX11EffectShaderResourceVariable* texResource = var->AsShaderResource();
                Texture* tex = TextureManager::singleton()->getTexture(iter->second);
                if(tex)
                {
                    texResource->SetResource(tex->getShaderResourceView());
				}
				else{
					texResource->SetResource(0);
				}
            }
        }

        return true;
    }



   
}