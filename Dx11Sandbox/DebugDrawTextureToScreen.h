#ifndef DX11SANDBOX_DEBUGDRAWTEXTURETOSCREEN_H
#define DX11SANDBOX_DEBUGDRAWTEXTURETOSCREEN_H

#include "DebugDrawer.h"
#include "RCObjectPtr.h"
#include "CommonTypes.h"
namespace Dx11Sandbox
{
	class Mesh;


	class DebugDrawTextureToScreen : public DebugDrawer
	{
	public:
		DebugDrawTextureToScreen(void);
		virtual ~DebugDrawTextureToScreen(void);

		virtual void Draw(RenderContext* state);


	protected:
		

	};
};

#endif

