#include "DebugDrawLights.h"
#include "BasicForwardRenderer.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "MaterialManager.h"
#include "Material.h"
#include "Shader.h"
#include "MeshUtility.h"
#include "RenderBin.h"
#include "ILDescriptions.h"
#include "Light.h"
#include "Mesh.h"
#include "RenderCamera.h"
#include "d3dx11effect.h"

namespace Dx11Sandbox
{

	const string DebugDrawLights::s_debugMaterialName = "unlitTexture";

	


	DebugDrawLights::DebugDrawLights(ID3D11Device *device)
		:m_renderer(new BasicForwardRenderer),
		m_renderData(new RenderData),
		m_device(device),
		m_enabled(false)
	{

		Mesh* mesh = MeshUtility::createUnitSphere(device, 25, 20, false, false, false, true);
		Material* mat = MaterialManager::singleton()->getOrCreateMaterial(device, "gizmoColored.fx", "gizmo", mesh->getInputLayout());
		m_renderData->setMaterial(mat);
		m_renderData->setMesh(mesh);
		
	}

	void DebugDrawLights::setEnabled(bool value){
		m_enabled = value;
	}

	bool DebugDrawLights::isEnabled(){
		return m_enabled;
	}

	void DebugDrawLights::draw(SceneManager* mngr, RenderContext* state)
	{
		if (!m_enabled) return;

		m_renderer->renderBegin(mngr->getMainCamera(), 0, state);
		RenderBin& rb = mngr->getRenderBin();
		std::vector<Light*>& lightList = rb.getLights();
		int visibleOmnis = 0;
		
		Matrix mat;
		Vec4 color;
		matMakeIdentity(mat);
		ID3DX11EffectVectorVariable* colorVar = m_renderData->getMaterial()->getShader()->getEffect()->GetVariableByName("color")->AsVector();
		for (unsigned int i = 0; i < lightList.size(); ++i){
			if (lightList[i]->getLightType() != Light::OMNI) continue;
			++visibleOmnis;
			Light* omniLight = lightList[i];
			const Vec4& p = omniLight->getPosition();
			mat[0][0] = p[3];
			mat[1][1] = p[3];
			mat[2][2] = p[3];
			mat[3][0] = p[0];
			mat[3][1] = p[1];
			mat[3][2] = p[2];

			color = omniLight->getColor();
			color[3] = 1.f;
			colorVar->SetFloatVector((float*)(&color));
			m_renderData->setWorldMatrix(mat);
			m_renderer->render(&m_renderData, 1);

		}

		m_renderer->renderEnd();
	}


	DebugDrawLights::~DebugDrawLights()
	{
		SAFE_DELETE(m_renderData);
	}
}