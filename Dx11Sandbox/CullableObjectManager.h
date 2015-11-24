#ifndef DX11SANDBOX_CULLABLEOBJECTMANAGER_H
#define DX11SANDBOX_CULLABLEOBJECTMANAGER_H


#include <set>
#include <map>
#include "CullDataAllocator.h"


namespace Dx11Sandbox
{
	class CullableObjectManager
	{

	public:
		CullableObjectManager();
		virtual ~CullableObjectManager();

		template<class T> 
		T* create();
		
		CullDataAllocator* GetCullDataAllocator(RenderLayer layer);
		std::map<RenderLayer, CullDataAllocator*>& GetCullDataAllocators();

		void destroy(Cullable* obj);
		void destroyAll();
		
	protected:
		std::map<RenderLayer, CullDataAllocator*> m_cullDataLayers;
		std::set<Cullable*> m_cullableObjects;

	};

	

	template<class T>
	T* CullableObjectManager::create()
	{
		T* t = new T(this);
		m_cullableObjects.insert(t);
		return t;
	}

}
#endif
