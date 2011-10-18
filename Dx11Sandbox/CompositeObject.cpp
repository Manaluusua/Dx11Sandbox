/*#include "CompositeObject.h"
#include "DXUT.h"
#include "Material.h"
#include "TextureManager.h"
#include "Texture.h"
#include "DXUTcamera.h"
#include <d3dx11effect.h>
#include "Mesh.h"

namespace Dx11Sandbox
{

    CompositeObject::CompositeObject()
        :m_indexData(0),
        m_sharedVertexBuffer(0),
        m_subObjectCount(0)
    {
        
    }


    CompositeObject::~CompositeObject(void)
    {
        if(m_indexData)
        {
            delete [] m_indexData;
        }
        if(m_sharedVertexBuffer)
        {
            delete m_sharedVertexBuffer;
        }
    }

   void CompositeObject::setIndexBuffers( Mesh* indexes,  unsigned int numObj)
   {
       if(m_indexData)
       {
           delete [] m_indexData;
       }
       m_indexData = indexes;
       m_subObjectCount = numObj;
   }
   void CompositeObject::setVertexBuffers( Mesh* vertices)
   {
       if(m_sharedVertexBuffer)
       {
           delete m_sharedVertexBuffer;
       }
       m_sharedVertexBuffer = vertices;
   }

        

    void CompositeObject::render(ID3D11Device* pd3dDevice,ID3D11DeviceContext* context, const CBaseCamera* cam, double fTime, float fElapsedTime, Material* forcemat)
    {
        if(!m_indexData || !m_sharedVertexBuffer || !m_material)
            return;

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
        ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(0);
        tech->GetDesc(&techDesc);

        tech->GetPassByIndex(0)->GetDesc(&passDesc);
        
        context->IASetInputLayout( mat->getInputLayout() );

        // Get the projection & view matrix from the camera class
        view = *cam->GetViewMatrix();
        proj = *cam->GetProjMatrix();
        worldViewProjection = m_world *view * proj;

       effect->GetVariableByName( "worldViewProj" )->AsMatrix()->SetMatrix((float*)&worldViewProjection);
  
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
        UINT* strides = &m_sharedVertexBuffer->getVertexBuffer().stride;
        UINT offsets[1];
        offsets[0] = 0;
        ID3D11Buffer *buffers[1];
        buffers[0] = m_sharedVertexBuffer->getVertexBuffer().buffer;

        context->IASetVertexBuffers(0,1,buffers,strides, offsets);
        context->IASetPrimitiveTopology( m_sharedVertexBuffer->getPrimType() );
        for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
        {
            tech->GetPassByIndex(passInd)->Apply(0, context);
            for(UINT submeshes = 0; submeshes < m_subObjectCount; ++submeshes)
            {
                context->IASetIndexBuffer(m_indexData[submeshes].getIndexBuffer().buffer,m_indexData[submeshes].getIndexBuffer().format, 0);
                context->DrawIndexed(m_indexData[submeshes].getIndexBuffer().indexCount, 0, 0);
            }
        }
    
        
    }
}*/