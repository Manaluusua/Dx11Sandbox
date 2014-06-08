#ifndef DX11SANDBOX_DEBUGDRAWLIGHTS_H
#define DX11SANDBOX_DEBUGDRAWLIGHTS_H

#include "DebugDrawer.h"
#include "RCObjectPtr.h"
#include "CommonTypes.h"
#include "Camera.h"
#include <vector>


namespace Dx11Sandbox
{
	class RenderData;
	class Renderer;
	class DebugDrawLights : public DebugDrawer
	{
	public:
		DebugDrawLights(ID3D11Device *device);
		virtual ~DebugDrawLights();

		virtual void draw(SceneManager* mngr, RenderContext* state);
		void setEnabled(bool value);
		bool isEnabled();
	private:
		DISABLE_COPY(DebugDrawLights);

		static const string s_debugMaterialName;
		RCObjectPtr<Renderer> m_renderer;
		RenderData* m_renderData;
		ID3D11Device* m_device;
		bool m_enabled;
	};

}

#endif