#include "DebugDrawTextureToScreen.h"
#include "BasicForwardRenderer.h"
#include "Material.h"
#include "MaterialManager.h"
#include "MeshUtility.h"
#include "RenderData.h"
#include "Texture.h"
#include "RenderContext.h"

namespace Dx11Sandbox
{

	const string DebugDrawTextureToScreen::s_debugMaterialName = "unlitTexture";

	DebugDrawTextureToScreen::DebugDrawTextureToScreen(ID3D11Device *device, float width, float height)
		:m_device(device)
	{
		m_cam.setProjectionOrthographic(width, width/height, 0.1f, 100.f);
		MaterialManager::singleton()->getOrCreateMaterial(device, "unlitTexture.fx", s_debugMaterialName, MeshInputLayouts::POS3TEX2);
		m_renderer = new BasicForwardRenderer;
	}


	DebugDrawTextureToScreen::~DebugDrawTextureToScreen(void)
	{
		for(auto iter = m_debugData.begin(); iter != m_debugData.end() ; ++iter)
		{
			delete *iter;
		}
		m_debugData.clear();
	}

	void DebugDrawTextureToScreen::draw(SceneManager* mngr, RenderContext* state)
	{
		state->disableDepthStencil(true);
		m_renderer->renderBegin(&m_cam, 0, state);
		m_renderer->render(m_debugData.data(), m_debugData.size());
		m_renderer->renderEnd();



		state->disableDepthStencil(false);
	}



	void DebugDrawTextureToScreen::addDebugTexture(ResourceID textureId, float x, float y, float width, float height)
	{
		Mesh* mesh = MeshUtility::createQuad(m_device,x, y, width, height, false, true);
		RCObjectPtr<Material> mat = new Material(*MaterialManager::singleton()->getMaterial(s_debugMaterialName));
		mat->setTexture("tex", textureId);
		RenderData* data = new RenderData;
		data->setMesh(mesh);
		data->setMaterial(mat);

		m_debugData.push_back(data);
	}

	void DebugDrawTextureToScreen::addDebugTexture(Texture* tex, float x, float y, float width, float height)
	{
		addDebugTexture(tex->getName(), x, y, width, height);
	}

	void DebugDrawTextureToScreen::addDebugTexture(const string& textureName, float x, float y, float width, float height)
	{
		addDebugTexture(stringToID(textureName), x, y, width, height);

	}
}