#include "RenderObject.h"
#include "DXUT.h"
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "DXUTcamera.h"
#include <d3dx11effect.h>
#include "Mesh.h"
namespace Dx11Sandbox
{
    RenderObject::RenderObject( Mesh* mesh, Material* material)
        :m_material(material),
        m_mesh(mesh),
        m_renderObjectMask(0xFFFFFFFF)
    {

    }
    RenderObject::~RenderObject()
    {
        SAFE_DELETE(m_material);
        SAFE_DELETE(m_mesh);
          
    }

    void RenderObject::render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context,const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat)
    {
        D3DXMATRIXA16 world;
        D3DXMATRIXA16 view;
        D3DXMATRIXA16 proj;
        D3DXMATRIXA16 worldViewProjection;
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        ID3DX11Effect* effect;
   
        Material* mat = forcemat==0?m_material:forcemat;
        std::map<string, wstring> &textures = mat->getTextureReferences();


        
        effect = mat->getEffect();
        effect->GetDesc(&effectDesc);
        
        //for now use first tech
        ID3DX11EffectTechnique* tech = effect->GetTechniqueByName("Basic");
        tech->GetDesc(&techDesc);

        tech->GetPassByIndex(0)->GetDesc(&passDesc);
        
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

        //for now a mesh contains no submeshes, and only one vertexbuffer is set
        //TO DO: make this more general and get primitive types etc. from mesh
        UINT* strides = &m_mesh->getVertexBuffer().stride;
        UINT offsets[1];
        offsets[0] = 0;
        ID3D11Buffer *buffers[1];
        buffers[0] = m_mesh->getVertexBuffer().buffer;

        context->IASetVertexBuffers(0,1,buffers,strides, offsets);

        context->IASetIndexBuffer(m_mesh->getIndexBuffer().buffer,m_mesh->getIndexBuffer().format, 0);


        context->IASetPrimitiveTopology( m_mesh->getPrimType() );

 
        for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
        {
            tech->GetPassByIndex(passInd)->Apply(0, context);
            context->DrawIndexed(m_mesh->getIndexBuffer().indexCount, 0, 0);
        }

    }

}