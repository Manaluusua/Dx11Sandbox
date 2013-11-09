#ifndef DX11SANDBOX_RENDEROBJECTMANAGER_H
#define DX11SANDBOX_RENDEROBJECTMANAGER_H


#include <set>
#include "CullInfoManager.h"

namespace Dx11Sandbox
{
	
	class CullableGeometry;
	
	class RenderObjectManager
	{
		friend class CullableGeometry;
	public:
		RenderObjectManager();
		virtual ~RenderObjectManager();
		CullableGeometry* CreateRenderObject();
		
		CullInfoManager& GetCullingManager();

		void DestroyAllRenderObjects();

	protected:
		
		void DestroyRenderObject(CullableGeometry* obj);
		
		CullInfoManager m_cullingInfoManager;
		std::set<CullableGeometry*> m_renderObjects;

	};

}
#endif
