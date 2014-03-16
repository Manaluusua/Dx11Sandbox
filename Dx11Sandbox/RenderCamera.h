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
		virtual void cameraPreCull(RenderCamera& camera) = 0;
		virtual void cameraStartedRendering(RenderCamera& camera, RenderBin& renderbin, RenderContext* state) = 0; 
		virtual void cameraEndedRendering(RenderCamera& camera, RenderBin& renderbin, RenderContext* state) = 0;
	};

	class RenderCamera :
		public Camera
	{
		friend class SceneManager;
	public:
		
		RenderCamera(void);
		virtual ~RenderCamera(void);

		void render(RenderBin& renderBin, RenderContext* state);

		void setCameraPriority(INT32 priority);
		INT32 getCameraPriority() const;

		void setRenderMask(RenderLayer mask);
		RenderLayer getRenderMask() const;

		void addRenderListener(RenderCameraListener *l);
		void removeRenderListener(RenderCameraListener *l);

		

		void startedCulling();

		void setRenderer(RCObjectPtr<Renderer> renderer);
		RCObjectPtr<Renderer> getRenderer() const;
	protected:

		std::set<RenderCameraListener*> m_renderListeners;
		RCObjectPtr<Renderer> m_renderer;

		INT32 m_cameraPriority;
		RenderLayer m_renderMask;
	private:
		DISABLE_COPY(RenderCamera)
		
	};
};
#endif