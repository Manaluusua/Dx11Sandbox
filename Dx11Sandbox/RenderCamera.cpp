#include "RenderCamera.h"

namespace Dx11Sandbox
{

	RenderCamera::RenderCamera(void)
		:m_cameraPriority(0),
		m_renderMask(0xFFFFFFFF)
	{

	}
	 
	RenderCamera::~RenderCamera(void)
	{

	}

	void RenderCamera::setCameraPriority(INT32 priority)
	{
		m_cameraPriority = priority;
	}

	INT32 RenderCamera::getCameraPriority() const
	{
		return m_cameraPriority;
	}

	void RenderCamera::setRenderMask(RenderMask mask){
		m_renderMask = mask;
	}

	RenderMask RenderCamera::getRenderMask() const
	{
			return m_renderMask;
	}

};
