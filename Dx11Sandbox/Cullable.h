#ifndef DX11SANDBOX_CULLABLE_H
#define DX11SANDBOX_CULLABLE_H

namespace Dx11Sandbox
{


	class RenderPrimitiveGatherer;

	class Cullable
	{
	public:

		Cullable(void);
		virtual ~Cullable(void);


		virtual void passedCulling(RenderPrimitiveGatherer *gatherer) = 0;

	};

};
#endif