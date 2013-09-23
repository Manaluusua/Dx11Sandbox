#ifndef DX11SANDBOX_RENDERCAMERA_H
#define DX11SANDBOX_RENDERCAMERA_H

#include "Camera.h"
namespace Dx11Sandbox
{

	class RenderCamera :
		public Camera
	{
		friend class SceneManager;
	public:
		
		

		void setCameraPriority(INT32 priority);
		INT32 getCameraPriority() const;

		void setRenderMask(RenderMask mask);
		RenderMask getRenderMask() const;

	protected:
		
		DISABLE_COPY(RenderCamera)
		RenderCamera(void);
		virtual ~RenderCamera(void);


		INT32 m_cameraPriority;
		RenderMask m_renderMask;

	};
};
#endif