#ifndef DX11SANDBOX_DEBUGDRAWER_H
#define DX11SANDBOX_DEBUGDRAWER_H
namespace Dx11Sandbox
{

	class RenderContext;
	class SceneManager;
	class DebugDrawer
	{
	public:
		virtual void draw(SceneManager* mngr, RenderContext* state) = 0;
		
		virtual ~DebugDrawer(void);
		
	};
}
#endif