#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "RenderContext.h"
#include "BasicForwardRenderer.h"
#include "EnvironmentInfo.h"
#include "Mesh.h"
#include "MeshUtility.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Shader.h"
#include "RenderData.h"
#include "TextureManager.h"
#include "Texture.h"
#include "d3dx11effect.h"

#include <D3D11.h>
namespace Dx11Sandbox
{

	const unsigned int DeferredRenderer::s_threadsPerGroupX = 16;
	const unsigned int DeferredRenderer::s_threadsPerGroupY = 16;
	const string DeferredRenderer::s_copyMaterialName = "unlitTexture";

	DeferredRenderer::DeferredRenderer(RenderContext* state)
		:m_forwardRenderer(new BasicForwardRenderer),
		m_state(state),
		m_lights(0),
		m_cam(0),
		m_gbuffer(new GBuffer(state)),
		m_accumulateLightsCS(0),
		m_copyData(new RenderData),
		m_lightingOutput(0),
		m_gbufferBound(false),
		m_deferredPathActive(false)
	{

		m_copyData->setMesh(MeshUtility::createQuad(m_state->getDevice(), 0, 0, 2, 2, false, true));
		m_copyData->setMaterial( MaterialManager::singleton()->getOrCreateMaterial(m_state->getDevice(), "unlitTexture.fx", s_copyMaterialName, MeshInputLayouts::POS3TEX2) );
		m_accumulateLightsCS = new Shader(MaterialManager::singleton()->getAssetPath() + "calculateLighting.fx",state->getDevice());
	}


	DeferredRenderer::~DeferredRenderer()
	{
		SAFE_DELETE(m_copyData);
	}

	void DeferredRenderer::createOutputTex(unsigned int w, unsigned int h)
	{
		if (!m_lightingOutput){
			m_lightingOutput = TextureManager::singleton()->createTexture("DeferredRendererOutput");
		}
		if (m_lightingOutput->getWidth() == w && m_lightingOutput->getHeight() == h) return;		
		m_lightingOutput->createResource(m_state->getDevice(), w, h, true, D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);
	}

	
	void DeferredRenderer::bindGBuffer()
	{
		if (m_gbufferBound) return;

		//figure out later where to get the gbuffer dimensions
		createOutputTex(EnvironmentInfo::getScreenWidth(), EnvironmentInfo::getScreenHeight());
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
			copyLightPassOutputToRenderTarget();
		}
	}


	

	void DeferredRenderer::renderToGBuffer(RenderData** objects, unsigned int objectCount)
	{

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

	void  DeferredRenderer::copyLightPassOutputToRenderTarget()
	{
		
		m_copyData->getMaterial()->setTexture("tex", m_lightingOutput->getName());
		D3DXMATRIX mat = *m_cam->getViewMatrix() * *m_cam->getProjectionMatrix();
		float det;
		D3DXMatrixInverse(&mat,&det, &mat);
		m_copyData->setWorldMatrix(mat);


		RenderData* d[1];
		d[0] = m_copyData;
		m_forwardRenderer->render(d, 1);
		
		m_state->disableDepthStencil(false);
	}

	void DeferredRenderer::doLightPass()
	{
		unbindGBuffer();
		m_state->disableDepthStencil(true);

		ID3DX11Effect* effect = m_accumulateLightsCS->getEffect();
		effect->GetVariableByName("albedo")->AsShaderResource()->SetResource(m_gbuffer->getBufferTexture(GBuffer::ALBEDO)->getShaderResourceView());
		effect->GetVariableByName("normal")->AsShaderResource()->SetResource(m_gbuffer->getBufferTexture(GBuffer::NORMAL)->getShaderResourceView());
		effect->GetVariableByName("specular")->AsShaderResource()->SetResource(m_gbuffer->getBufferTexture(GBuffer::SPECULAR)->getShaderResourceView());
		effect->GetVariableByName("depth")->AsShaderResource()->SetResource(m_state->getDefaultDepthStencilTexture()->getShaderResourceView());
		effect->GetVariableByName("output")->AsUnorderedAccessView()->SetUnorderedAccessView(m_lightingOutput->getUnorderedAccessView());
		effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_state->getImmediateContext());

		int threadsX = m_gbuffer->getWidth() / s_threadsPerGroupX + ( (m_gbuffer->getWidth() % s_threadsPerGroupX) > 0 ? 1 : 0);
		int threadsY = m_gbuffer->getHeight() / s_threadsPerGroupY + ((m_gbuffer->getHeight() % s_threadsPerGroupY) > 0 ? 1 : 0);

		m_state->getImmediateContext()->Dispatch(threadsX, threadsY, 1);

		effect->GetVariableByName("albedo")->AsShaderResource()->SetResource(0);
		effect->GetVariableByName("normal")->AsShaderResource()->SetResource(0);
		effect->GetVariableByName("specular")->AsShaderResource()->SetResource(0);
		effect->GetVariableByName("depth")->AsShaderResource()->SetResource(0);
		effect->GetVariableByName("output")->AsUnorderedAccessView()->SetUnorderedAccessView(0);
		effect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_state->getImmediateContext());



		
	}
}