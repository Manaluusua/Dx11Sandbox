#ifndef DX11SANDBOX_ENVIRONMENTCHANGEDLISTENER_H
#define DX11SANDBOX_ENVIRONMENTCHANGEDLISTENER_H

namespace Dx11Sandbox
{

	class SceneManager;

	class EnvironmentChangedListeners
	{
	public:

		virtual void renderWindowResized(SceneManager* mngr) = 0;


		virtual ~EnvironmentChangedListeners(void);
	};

}

#endif