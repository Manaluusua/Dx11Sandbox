#include "BasicForwardRenderer.h"
#include "RenderContext.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Shader.h"
#include "RenderData.h"
#include "RenderCamera.h"
#include "d3dx11effect.h"
#include <D3D11.h>

namespace Dx11Sandbox
{
	BasicForwardRenderer::BasicForwardRenderer(void)
		:m_state(0)
	{
	}


	BasicForwardRenderer::~BasicForwardRenderer(void)
	{
	}


	void BasicForwardRenderer::renderBegin(Camera* cam,std::vector<Light*>* lights,RenderContext* state)
	{
		m_state = state;
		m_cam = cam;

		m_materialPropertySetter.setCurrentCamera(cam);
		m_materialPropertySetter.setLights(lights);
	}

	void BasicForwardRenderer::renderEnd()
	{
		m_state =  0;
	}

	void BasicForwardRenderer::renderingQueue(RenderQueueID id)
	{

	}

	void BasicForwardRenderer::render(RenderData** objects, unsigned int objectCount)
	{
        D3DX11_TECHNIQUE_DESC techDesc;
        D3DX11_PASS_DESC passDesc;

        for( unsigned int i = 0; i < objectCount; ++i )
        {
			RenderData* object = objects[i];

			Mesh* mesh = object->getMesh();
			Material* mat = object->getMaterial();
        
            if(!mesh || !mat )
                return;
			
			m_materialPropertySetter.setShaderUniforms(object,mat);

            ID3DX11Effect* effect = mat->getShader()->getEffect();
            ID3D11DeviceContext* context = m_state->getImmediateContext();

        
            
            ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(Renderer::FORWARD_RENDERER);
            tech->GetDesc(&techDesc);

        



            m_state->bindMesh(mesh);
            m_state->bindMaterial(mat);

            
        
        

            for ( uint32_t passInd = 0; passInd < techDesc.Passes; ++passInd )
            {
                tech->GetPassByIndex(passInd)->Apply(0, context );
                if(mesh->getIndexBuffer()->getIndexCount() > 0)
                {
                    context->DrawIndexed(mesh->getIndicesToDrawCount(), mesh->getIndicesToDrawOffset(), 0);
                }else
                {
                    context->Draw(mesh->getVertexBuffer()->getVertexCount(),0);
                }
            }
        }
	}

}