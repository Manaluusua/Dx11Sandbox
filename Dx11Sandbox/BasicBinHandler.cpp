#include "BasicBinHandler.h"
#include "CullInfo.h"

#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include <D3D11.h>
#include <d3dx9math.h>
#include "d3dx11effect.h"
#include "Camera.h"
#include "RenderObject.h"

namespace Dx11Sandbox
{
    BasicBinHandler::BasicBinHandler(void)
    {
    }


    BasicBinHandler::~BasicBinHandler(void)
    {
    }


	void BasicBinHandler::setupForRendering(RenderObject* objects, unsigned int objectCount, RenderData** objectsOut, unsigned int *objectsOutCount, RenderContext* state)
	{
		*objectsOut = static_cast<RenderData*>(objects);
		*objectsOutCount = objectCount;
	}

	/*
    void BasicBinHandler::render(std::vector<CullInfo*>& objects, RenderContext* state, Camera* camera)
    {


        D3DXMATRIXA16 worldViewProjection;
        D3DX11_EFFECT_DESC effectDesc;
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        for( unsigned int i = 0; i < objects.size(); ++i )
        {
            const CullInfo* cullingInfo = objects[i];
			RenderObject* object = cullingInfo->object;

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
    }*/
}
