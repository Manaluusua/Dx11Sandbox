#ifndef DX11SANDBOX_RENDERCAMERA_H
#define DX11SANDBOX_RENDERCAMERA_H

#include "Camera.h"
#include "RCObjectPtr.h"
#include <set>

namespace Dx11Sandbox
{
	class Renderer;
	class RenderBin;
	class RenderContext;
	class RenderCamera;

	class RenderCameraListener
	{
	public:
		virtual void cameraStartedRendering(RenderCamera& camera, RenderBin& renderbin, RenderContext* state); 

	};

	class RenderCamera :
		public Camera
	{
		friend class SceneManager;
	public:
		
		void render(RenderBin& renderBin, RenderContext* state);

		void setCameraPriority(INT32 priority);
		INT32 getCameraPriority() const;

		void setRenderMask(RenderLayer mask);
		RenderLayer getRenderMask() const;

		void addRenderListener(RenderCameraListener *l);
		void removeRenderListener(RenderCameraListener *l);

		void setRenderer(RCObjectPtr<Renderer> renderer);
		RCObjectPtr<Renderer> getRenderer() const;
	protected:
		
		std::set<RenderCameraListener*> m_renderListeners;

		DISABLE_COPY(RenderCamera)
		RenderCamera(void);
		virtual ~RenderCamera(void);

		RCObjectPtr<Renderer> m_renderer;

		INT32 m_cameraPriority;
		RenderLayer m_renderMask;

	};
};
#endif