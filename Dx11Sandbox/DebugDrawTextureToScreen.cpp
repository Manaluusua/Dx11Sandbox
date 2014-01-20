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

	const string DebugDrawTextureToScreen::debugMaterialName = "unlitTexture";

	DebugDrawTextureToScreen::DebugDrawTextureToScreen(ID3D11Device *device, float width, float height)
		:m_device(device)
	{
		m_cam.setProjectionOrthographic(width, width/height, 0.1f, 100.f);
		MaterialManager::singleton()->getOrCreateMaterial(device, "unlitTexture.fx", debugMaterialName, MeshInputLayouts::POS3TEX2);
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

	void DebugDrawTextureToScreen::draw(RenderContext* state)
	{
		
		m_renderer->renderBegin(&m_cam, 0, state);
		m_renderer->render(m_debugData.data(), m_debugData.size());
		m_renderer->renderEnd();
	}

	Mesh* DebugDrawTextureToScreen::createQuad(float x, float y, float w, float h)
	{
		
		
		D3DXVECTOR3 corners[4];
		float depth = 0.2f;

		float hw = w*0.5f;
		float hh = h*0.5f;

		for(int i = 0; i < 4; ++i)
		{
			corners[i] = D3DXVECTOR3( x - hw + (i%2) * w, y - hh + (i/2) * h, depth);
		}
		return MeshUtility::createQuad(m_device, corners);

	}

	void DebugDrawTextureToScreen::addDebugTexture(Texture* tex, float x, float y, float width, float height)
	{
		Mesh* mesh = createQuad(x, y, width, height);
		RCObjectPtr<Material> mat = new Material( *MaterialManager::singleton()->getMaterial(debugMaterialName) );
		mat->setTexture("tex", tex->getName());
		RenderData* data = new RenderData;
		data->setMesh(mesh);
		data->setMaterial(mat);

		m_debugData.push_back(data);
	}
}