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

    void BasicRenderer::render(std::vector<const RenderObject*>& objects, RenderContext* state, Camera* camera)
    {

        D3DXMATRIX *world;
        const D3DXMATRIX *view = camera->getViewMatrix();
        const D3DXMATRIX *proj = camera->getProjectionMatrix();
        D3DXMATRIXA16 worldViewProjection;
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        for(size_t i=0;i<objects.size();++i)
        {

            Mesh* mesh = objects[i]->mesh;
            Material* mat = objects[i]->mat;
        
            if(!mesh || !mat || !(rendererMask & objects[i]->renderObjectMask) )
                continue;
        
            ID3DX11Effect* effect = mat->getEffect();
            ID3D11DeviceContext* context = state->getImmediateContext();




        
        
            effect->GetDesc(&effectDesc);
        
            //for now use first tech
            ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(0);
            tech->GetDesc(&techDesc);

            tech->GetPassByIndex(0)->GetDesc(&passDesc);
        

            // Get the projection & view matrix from the camera class
            worldViewProjection = (*view) * (*proj);

            state->bindMesh(mesh);
            state->bindMaterial(mat);

            effect->GetVariableByName( "worldViewProj" )->AsMatrix()->SetMatrix((float*)&worldViewProjection);

            for ( UINT passInd = 0; passInd < techDesc.Passes; ++passInd )
            {
                tech->GetPassByIndex(passInd)->Apply(0, context );
                context->DrawIndexed(mesh->getIndexBuffer().indexCount, 0, 0);
            }
        }
    }
}
