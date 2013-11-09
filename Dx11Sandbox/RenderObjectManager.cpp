#include "RenderObjectManager.h"

#include "CullableGeometry.h"

namespace Dx11Sandbox
{
	RenderObjectManager::RenderObjectManager(void)
	{
	}


	RenderObjectManager::~RenderObjectManager(void)
	{
	}

	CullableGeometry* RenderObjectManager::CreateRenderObject()
	{
		CullableGeometry* ro = new CullableGeometry(this);
		m_renderObjects.insert(ro);
		return ro;
	}

	CullInfoManager& RenderObjectManager::GetCullingManager()
	{
		return m_cullingInfoManager;
	}

	void RenderObjectManager::DestroyRenderObject(CullableGeometry* obj)
	{
		if(obj == 0) return;

		if(m_renderObjects.find(obj) == m_renderObjects.end()) return;
		m_renderObjects.erase(obj);
		delete obj;
	}
	void RenderObjectManager::DestroyAllRenderObjects()
	{
		for(auto iter = m_renderObjects.begin(); iter != m_renderObjects.end(); ++iter)
		{
			delete *iter;
		}
		m_renderObjects.clear();

		m_cullingInfoManager.deallocateDynamicAll();
	}
}