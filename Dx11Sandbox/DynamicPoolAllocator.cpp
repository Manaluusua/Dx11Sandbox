#include "DynamicPoolAllocator.h"
#include <algorithm>
#include <cstddef>
namespace Dx11Sandbox
{
    template <typename T>
    DynamicPoolAllocator<T>::DynamicPoolAllocator(unsigned int defaultSize)
        :m_indexPool(0),
        m_headPool(0),
        m_defaultAllocSize(defaultSize),
        m_indexProxy(0),
        m_headProxy(0)
    {
        m_proxy.resize(1);
        m_proxy[m_indexProxy].resize(m_defaultAllocSize);
        m_headProxy = &m_proxy[m_indexProxy][0];

        m_pool.resize(1);
        m_pool[m_indexPool].vector.resize(m_defaultAllocSize);
        m_headPool = &m_pool[m_indexPool].vector[0];
    }

    template <typename T>
    DynamicPoolAllocator<T>::~DynamicPoolAllocator(void)
    {
    }


    template <typename T>
    T** DynamicPoolAllocator<T>::getProxy()
    {
        T** proxy = 0;
        if(!m_proxyRecycle.empty())
        {
            proxy = m_proxyRecycle.back();
            m_proxyRecycle.pop_back();

        }else
        {
            AllocationUnit<T>** end = &m_proxy[m_indexProxy][0] + m_proxy[m_indexProxy].size();
            if(m_headProxy == end)
            {
                m_proxy.resize(m_proxy.size() + 1);
                ++m_indexProxy;
                m_proxy[m_indexProxy].resize(m_defaultAllocSize);
                m_headProxy = &m_proxy[m_indexProxy][0];
            }
            proxy = m_headProxy;
            ++m_headProxy;  
        }
    }

    template <typename T>
    void DynamicPoolAllocator<T>::deallocateProxy(T** proxy)
    {
        *proxy = 0;
        m_proxyRecycle.push_back(proxy);
    }

    template <typename T>
    T** DynamicPoolAllocator<T>::allocateDynamic()
    {
        T** proxy = getProxy();

        AllocationUnit<T>* end = &m_pool[m_indexPool].vector[0] + m_pool[m_indexPool].vector.size();
        if(m_headProxy == end)
        {
            m_pool.resize(m_pool.size() + 1);
            ++m_indexPool;
            m_pool[m_indexPool].vector.resize(m_defaultAllocSize);
            m_headPool = &m_pool[m_indexPool].vector[0];
        }
        m_pool[m_indexPool].count += 1; 
        m_headPool->proxy = proxy;
        *proxy = &m_headPool->data;
        ++m_headPool;
    }

    template <typename T>
    void DynamicPoolAllocator<T>::deallocateDynamic(T** obj)
    {
        AllocationUnit<T>* unit = ((reinterpret_cast<void*>(*obj)) - offsetof(AllocationUnit<T>,data));
        deallocateProxy(obj);
        if(m_headPool == m_pool[m_indexPool].vector[0])
        {
            --m_indexPool;
            m_headPool = &m_pool[m_indexPool].vector.back();
        }else
        {
            --m_headPool;
        }
        if(unit!=m_headPool)
        {
            std::swap(*unit, *m_headPool);
            (*unit->proxy) = &unit;
        }
        m_pool[m_indexPool].count -= 1;
    }

}