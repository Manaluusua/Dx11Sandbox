#include "RenderData.h"

namespace Dx11Sandbox
{

	RenderData::RenderData(void)
		:m_mesh(0),
		m_material(0)
	{
		matMakeIdentity(m_worldMatrix);
	}


	RenderData::~RenderData(void)
	{
	}


}