#include "RenderObject.h"
#include "DXUT.h"
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "DXUTcamera.h"
#include <d3dx11effect.h>

namespace Dx11Sandbox
{
    RenderObject::RenderObject( Mesh* mesh, Material* material)
        :m_material(material),
        m_mesh(mesh)
    {

    }
    RenderObject::~RenderObject()
    {
        SAFE_DELETE(m_material);
        SAFE_DELETE(m_mesh);
          
    }

    void RenderObject::render(ID3D11Device* pd3dDevice,const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat)
    {
        HRESULT hr;
        D3DXMATRIXA16 world;
        D3DXMATRIXA16 view;
        D3DXMATRIXA16 proj;
        D3DXMATRIXA16 worldViewProjection;
        UINT submeshes;
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        ID3DX11Effect* effect;
   
        Material* mat = forcemat==0?m_material:forcemat;
        std::map<string, wstring> &textures = mat->getTextureReferences();


        
        effect = mat->getEffect();
        effect->GetDesc(&effectDesc);
        
        //for now use first tech
        ID3DX11EffectTechnique* tech = effect->GetTechniqueByName("TexturedNoSpecular");
        tech->GetDesc(&techDesc);

        tech->GetPassByIndex(0)->GetDesc(&passDesc);
        
        //m_mesh->GetAttributeTable( NULL, &submeshes );
        ID3D11DeviceContext* context;
        pd3dDevice->GetImmediateContext(&context);


        context->IASetInputLayout( mat->getInputLayout() );

        // Get the projection & view matrix from the camera class
        
        view = *cam->GetViewMatrix();
        proj = *cam->GetProjMatrix();
        worldViewProjection =  view * proj;

        effect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix()->SetMatrix((float*)&worldViewProjection);
  
        for(std::map<string, wstring>::iterator iter = textures.begin(); iter!=textures.end(); ++iter)
        {
           
            ID3DX11EffectVariable* var = effect->GetVariableByName(iter->first.c_str());
            if(var->IsValid())
            {
                ID3DX11EffectShaderResourceVariable* texResource = var->AsShaderResource();
                Texture* tex = TextureManager::getSingleton()->getTexture(iter->second);
                if(tex)
                    texResource->SetResource(tex->GetShaderResourceView());
            }
        }
 
        for(unsigned int submesh=0;submesh<submeshes;++submesh)
        {
            for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
            {
                tech->GetPassByIndex(passInd)->Apply(0, context);
                //m_mesh->DrawSubset(submesh);
            }
        }



    }

}