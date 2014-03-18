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
#include "RenderCamera.h"
#include "d3dx11effect.h"
namespace Dx11Sandbox
{

	const string DebugDrawLights::s_debugMaterialName = "unlitTexture";

	


	DebugDrawLights::DebugDrawLights(ID3D11Device *device)
		:m_renderer(new BasicForwardRenderer),
		m_renderData(new RenderData),
		m_device(device)
		
	{
		Mesh* mesh = MeshUtility::createUnitSphere(device, 25, 20, false, false, true);
		Material* mat = MaterialManager::singleton()->getOrCreateMaterial(device, "gizmoColored.fx", "gizmo", MeshInputLayouts::POS3);
		m_renderData->setMaterial(mat);
		m_renderData->setMesh(mesh);
		
	}

	void DebugDrawLights::draw(SceneManager* mngr, RenderContext* state)
	{
		m_renderer->renderBegin(mngr->getMainCamera(), 0, state);
		RenderBin& rb = mngr->getRenderBin();
		std::vector<Light*>& lightList = rb.getLights();
		int visibleOmnis = 0;
		
		D3DXMATRIX wmatrix;
		D3DXVECTOR4 color;
		D3DXMatrixIdentity(&wmatrix);
		ID3DX11EffectVectorVariable* colorVar = m_renderData->getMaterial()->getShader()->getEffect()->GetVariableByName("color")->AsVector();
		for (unsigned int i = 0; i < lightList.size(); ++i){
			if (lightList[i]->getLightType() != Light::OMNI) continue;
			++visibleOmnis;
			Light* omniLight = lightList[i];
			const D3DXVECTOR4& p = omniLight->getPosition();
			wmatrix._11 = p.w;
			wmatrix._22 = p.w;
			wmatrix._33 = p.w;
			wmatrix._41 = p.x;
			wmatrix._42 = p.y;
			wmatrix._43 = p.z;
			color = omniLight->getColor();
			color.w = 1.f;
			colorVar->SetFloatVector((float*)color);
			m_renderData->setWorldMatrix(wmatrix);
			m_renderer->render(&m_renderData, 1);

		}

		m_renderer->renderEnd();
	}


	DebugDrawLights::~DebugDrawLights()
	{
		SAFE_DELETE(m_renderData);
	}
}