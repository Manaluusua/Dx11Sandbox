#include "CullableObjectManager.h"
#include "Cullable.h"
namespace Dx11Sandbox
{
	

	CullableObjectManager::CullableObjectManager(void)
	{
	}

	CullableObjectManager::~CullableObjectManager(void)
	{
	}


	CullDataAllocator* CullableObjectManager::GetCullDataAllocator(RenderLayer layer)
	{
		if(m_cullDataLayers.find(layer) == m_cullDataLayers.end())
		{
			m_cullDataLayers[layer] = new CullDataAllocator;
		}
		return m_cullDataLayers[layer];
	}

	std::map<RenderLayer, CullDataAllocator*>& CullableObjectManager::GetCullDataAllocators()
	{
		return m_cullDataLayers;
	}

	void CullableObjectManager::destroy(Cullable* obj)
	{
		if(obj == 0) return;

		if(m_cullableObjects.find(obj) == m_cullableObjects.end()) return;
		m_cullableObjects.erase(obj);
		delete obj;
	}

	void CullableObjectManager::destroyAll()
	{
		for(auto iter = m_cullableObjects.begin(); iter != m_cullableObjects.end(); ++iter)
		{
			delete *iter;
		}
		m_cullableObjects.clear();

		for(auto iter = m_cullDataLayers.begin(); iter != m_cullDataLayers.end(); ++iter)
		{
			delete iter->second;
		}
		m_cullDataLayers.clear();
	}
}

