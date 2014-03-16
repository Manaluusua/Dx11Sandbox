#ifndef DX11SANDBOX_DEBUGDRAWTEXTURETOSCREEN_H
#define DX11SANDBOX_DEBUGDRAWTEXTURETOSCREEN_H

#include "DebugDrawer.h"
#include "RCObjectPtr.h"
#include "CommonTypes.h"
#include "Camera.h"
#include <vector>

namespace Dx11Sandbox
{
	class Mesh;
	class Texture;
	class RenderData;
	class Material;
	class Renderer;

	class DebugDrawTextureToScreen : public DebugDrawer
	{
	public:
		DebugDrawTextureToScreen(ID3D11Device *device, float width, float height);
		virtual ~DebugDrawTextureToScreen(void);

		virtual void draw(SceneManager* mngr, RenderContext* state);

		void addDebugTexture(Texture* tex, float x, float y, float width, float height);  
		void addDebugTexture(ResourceID textureId, float x, float y, float width, float height);
		void addDebugTexture(const string& textureName, float x, float y, float width, float height);

	private:


		static const string s_debugMaterialName;
		Camera m_cam;
		std::vector<RenderData*> m_debugData;
		RCObjectPtr<Renderer> m_renderer;
		
		ID3D11Device* m_device;

	};
};

#endif

