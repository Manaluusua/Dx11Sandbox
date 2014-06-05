#ifndef DX11SANDBOX_GEOMETRY_H
#define DX11SANDBOX_GEOMETRY_H

#include "CommonTypes.h"

namespace Dx11Sandbox
{
	class RenderData;
	
	class Geometry 
	{
	public:
		virtual RenderQueueID getRenderQueue() const = 0;
		virtual RenderLayer getRenderMask() const = 0;
		virtual RenderData& getRenderData() = 0;
	};

};

#endif