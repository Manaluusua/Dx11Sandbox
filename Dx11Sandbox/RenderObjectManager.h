#ifndef DX11SANDBOX_RENDEROBJECTMANAGER_H
#define DX11SANDBOX_RENDEROBJECTMANAGER_H


#include <set>

namespace Dx11Sandbox
{
	
	class RenderObject;
	
	class RenderObjectManager
	{
		friend class RenderObject;
	public:
		RenderObjectManager();
		virtual ~RenderObjectManager();
		RenderObject* CreateRenderObject();
		

	protected:
		void DestroyAllRenderObjects();
		void DestroyRenderObject(RenderObject* obj);
		std::set<RenderObject*> m_renderObjects;

	};

}
#endif
