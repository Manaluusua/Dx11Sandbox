#include "BasicRenderer.h"
#include "RenderObject.h"

#include "RenderContext.h"
#include "Mesh.h"
#include "Material.h"
#include <D3D11.h>
#include <d3dx9math.h>
#include "d3dx11effect.h"
#include "Camera.h"
namespace Dx11Sandbox
{
    BasicRenderer::BasicRenderer(void)
    {
    }


    BasicRenderer::~BasicRenderer(void)
    {
    }

    void BasicRenderer::render(const RenderObject* object, RenderContext* state, Camera* camera)
    {


        D3DXMATRIXA16 worldViewProjection;
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

      

        Mesh* mesh = object->mesh;
        Material* mat = object->mat;
        
        if(!mesh || !mat || !(rendererMask & object->renderObjectMask) )
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
            if(mesh->getIndexBuffer().indexCount > 0)
            {
                context->DrawIndexed(mesh->getIndexBuffer().indexCount, 0, 0);
            }else
            {
                context->Draw(mesh->getVertexBuffer().numVertices,0);
            }
        }
        
    }
}
