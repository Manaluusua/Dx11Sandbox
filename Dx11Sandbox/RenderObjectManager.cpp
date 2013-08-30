#include "RenderObjectManager.h"

#include "RenderObject.h"

namespace Dx11Sandbox
{
	RenderObjectManager::RenderObjectManager(void)
	{
	}


	RenderObjectManager::~RenderObjectManager(void)
	{
	}

	RenderObject* RenderObjectManager::CreateRenderObject()
	{
		RenderObject* ro = new RenderObject;
		ro->m_mngr = this;
		m_renderObjects.insert(ro);
		return ro;
	}
	void RenderObjectManager::DestroyRenderObject(RenderObject* obj)
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
	}
}