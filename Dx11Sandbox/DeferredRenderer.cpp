#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "RenderContext.h"
#include "BasicForwardRenderer.h"
#include "EnvironmentInfo.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Shader.h"
#include "RenderData.h"
#include "d3dx11effect.h"
#include <D3D11.h>
namespace Dx11Sandbox
{

	DeferredRenderer::DeferredRenderer(RenderContext* state)
		:m_forwardRenderer(new BasicForwardRenderer),
		m_state(0),
		m_lights(0),
		m_cam(0),
		m_gbuffer(new GBuffer(state)),
		m_accumulateLightsCS(0),
		m_gbufferBound(false),
		m_deferredPathActive(false)
	{

		m_accumulateLightsCS = new Shader(MaterialManager::singleton()->getAssetPath() + "calculateLighting.fx",state->getDevice());
	}


	DeferredRenderer::~DeferredRenderer()
	{

	}

	void DeferredRenderer::bindGBuffer()
	{
		if (m_gbufferBound) return;

		//figure out later where to get the gbuffer dimensions
		m_gbuffer->allocateBuffer(EnvironmentInfo::getScreenWidth(), EnvironmentInfo::getScreenHeight());
		m_gbuffer->clear();
		m_gbuffer->setAsRenderTargets();

		m_gbufferBound = true;
	}

	void DeferredRenderer::unbindGBuffer()
	{
		if (!m_gbufferBound) return;
		m_state->popRenderTargets();
		m_gbufferBound = false;
	}

	void DeferredRenderer::renderBegin(Camera* cam, std::vector<Light*>* lights, RenderContext* state)
	{
		m_state = state;
		m_cam = cam;
		m_lights = lights;
		m_forwardRenderer->renderBegin(cam, lights, state);
		m_deferredPathActive = m_lights && m_lights->size() > 0u;

		if (m_deferredPathActive){
			bindGBuffer();
			m_materialPropertySetter.setCurrentCamera(cam);
		}
		


	}
	void DeferredRenderer::render(RenderData** objects, unsigned int objectCount)
	{
		if (!m_deferredPathActive){
			m_forwardRenderer->render(objects, objectCount);
			return;
		}
		renderToGBuffer(objects, objectCount);
	}
	void DeferredRenderer::renderEnd()
	{
		m_forwardRenderer->renderEnd();
	}

	

	void DeferredRenderer::renderingQueue(RenderQueueID id)
	{
		m_forwardRenderer->renderingQueue(id);

		//do light pass and switch to forward
		if (id >= RENDERQUEUE_AFTERLIGHTPASS && m_deferredPathActive){
			m_deferredPathActive = false;
			doLightPass();
		}
	}


	

	void DeferredRenderer::renderToGBuffer(RenderData** objects, unsigned int objectCount)
	{
		D3DX11_EFFECT_DESC effectDesc;
		D3DX11_TECHNIQUE_DESC techDesc;
		D3DX11_PASS_DESC passDesc;

		for (unsigned int i = 0; i < objectCount; ++i)
		{
			RenderData* object = objects[i];

			Mesh* mesh = object->getMesh();
			Material* mat = object->getMaterial();

			if (!mesh || !mat)
				return;

			m_materialPropertySetter.setShaderUniforms(object, mat,true, false);

			ID3DX11Effect* effect = mat->getShader()->getEffect();
			ID3D11DeviceContext* context = m_state->getImmediateContext();






			effect->GetDesc(&effectDesc);


			ID3DX11EffectTechnique* tech = effect->GetTechniqueByIndex(Renderer::DEFERRED_RENDERER);
			tech->GetDesc(&techDesc);





			m_state->bindMesh(mesh);
			m_state->bindMaterial(mat);

			

			for (UINT passInd = 0; passInd < techDesc.Passes; ++passInd)
			{
				tech->GetPassByIndex(passInd)->Apply(0, context);
				if (mesh->getIndexBuffer()->getIndexCount() > 0)
				{
					context->DrawIndexed(mesh->getIndicesToDrawCount(), mesh->getIndicesToDrawOffset(), 0);
				}
				else
				{
					context->Draw(mesh->getVertexBuffer()->getVertexCount(), 0);
				}
			}
		}
	}

	void DeferredRenderer::doLightPass()
	{
		unbindGBuffer();
	}
}