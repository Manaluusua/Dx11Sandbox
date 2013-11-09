#include "BasicForwardRenderer.h"
#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "RenderData.h"
#include "d3dx11effect.h"
#include <D3D11.h>
#include <d3dx9math.h>

namespace Dx11Sandbox
{
	BasicForwardRenderer::BasicForwardRenderer(void)
	{
	}


	BasicForwardRenderer::~BasicForwardRenderer(void)
	{
	}


	void BasicForwardRenderer::render(RenderData** objects, unsigned int objectCount, RenderContext* state)
	{
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        for( unsigned int i = 0; i < objectCount; ++i )
        {
			RenderData* object = *objects;

			Mesh* mesh = object->getMesh();
			Material* mat = object->getMaterial();
        
            if(!mesh || !mat )
                return;
        
            ID3DX11Effect* effect = mat->getEffect();
            ID3D11DeviceContext* context = state->getImmediateContext();


        

        
        
            effect->GetDesc(&effectDesc);
        
            //for now use first tech
            ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(0);
            tech->GetDesc(&techDesc);

            tech->GetPassByIndex(0)->GetDesc(&passDesc);
        



            state->bindMesh(mesh);
            state->bindMaterial(mat);

            
        
        

            for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
            {
                tech->GetPassByIndex(passInd)->Apply(0, context );
                if(mesh->getIndexBuffer()->getIndexCount() > 0)
                {
                    context->DrawIndexed(mesh->getIndexBuffer()->getIndexCount(), 0, 0);
                }else
                {
                    context->Draw(mesh->getVertexBuffer()->getVertexCount(),0);
                }
            }
        }
	}

}