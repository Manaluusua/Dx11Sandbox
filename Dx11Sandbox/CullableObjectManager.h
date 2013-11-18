#ifndef DX11SANDBOX_CULLABLEOBJECTMANAGER_H
#define DX11SANDBOX_CULLABLEOBJECTMANAGER_H


#include <set>
#include <map>
#include "CullDataAllocator.h"


namespace Dx11Sandbox
{

	template<class T>
	class CullableObjectManager
	{

	public:
		CullableObjectManager();
		virtual ~CullableObjectManager();
		T* create();
		
		CullDataAllocator* GetCullDataAllocator(RenderLayer layer);
		std::map<RenderLayer, CullDataAllocator*>& GetCullDataAllocators();

		void destroy(T* obj);
		void destroyAll();
		
	protected:
		std::map<RenderLayer, CullDataAllocator*> m_cullDataLayers;
		std::set<T*> m_cullableObjects;

	};

	template<class T>
	CullableObjectManager<T>::CullableObjectManager(void)
	{
	}

	template<class T>
	CullableObjectManager<T>::~CullableObjectManager(void)
	{
	}

	template<class T>
	T* CullableObjectManager<T>::create()
	{
		T* t = new T(this);
		m_cullableObjects.insert(t);
		return t;
	}

	template<class T>
	CullDataAllocator* CullableObjectManager<T>::GetCullDataAllocator(RenderLayer layer)
	{
		if(m_cullDataLayers.find(layer) == m_cullDataLayers.end())
		{
			m_cullDataLayers[layer] = new CullDataAllocator;
		}
		return m_cullDataLayers[layer];
	}

	template<class T>
	std::map<RenderLayer, CullDataAllocator*>& CullableObjectManager<T>::GetCullDataAllocators()
	{
		return m_cullDataLayers;
	}

	template<class T>
	void CullableObjectManager<T>::destroy(T* obj)
	{
		if(obj == 0) return;

		if(m_cullableObjects.find(obj) == m_cullableObjects.end()) return;
		m_cullableObjects.erase(obj);
		delete obj;
	}

	template<class T>
	void CullableObjectManager<T>::destroyAll()
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
#endif
