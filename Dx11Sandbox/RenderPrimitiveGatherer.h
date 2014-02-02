#ifndef DX11SANDBOX_RENDERPRIMITIVEGATHERER_H
#define DX11SANDBOX_RENDERPRIMITIVEGATHERER_H

namespace Dx11Sandbox
{
	class Geometry;
	class Light;
	class RenderPrimitiveGatherer
	{
	public:
		virtual void addGeometry(Geometry* geom) = 0;
		virtual void addLight(Light* light) = 0;
	};

};


#endif