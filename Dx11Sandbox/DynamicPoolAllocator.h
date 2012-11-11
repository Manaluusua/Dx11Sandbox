#ifndef DX11SANDBOX_DYNAMICPOOLALLOCATOR_H
#define DX11SANDBOX_DYNAMICPOOLALLOCATOR_H

#include <vector>
#include <cassert>
namespace Dx11Sandbox
{

    template <typename T>
    class AllocationUnit
    {
       template <typename T> friend class DynamicPoolAllocator;

       T **proxy;

    public:
        T data;

    };

    template <typename T>
    class PoolVector
    {
    public:
        PoolVector():count(0){}
   
        std::vector<T> vector;
        size_t count;
    };

    template <typename T>
    class DynamicPoolAllocator
    {
    private:

        std::vector<PoolVector<AllocationUnit<T> > > m_pool;

        std::vector<std::vector<T*> > m_proxy;
        std::vector<T**> m_proxyRecycle;

        size_t m_indexPool;
        AllocationUnit<T>* m_headPool;
        size_t m_defaultAllocSize;

        size_t m_indexProxy;
        T** m_headProxy;

        inline T** getProxy();
        inline void deallocateProxy(T** proxy);

    public:
        DynamicPoolAllocator(size_t defaultSize=40);
        ~DynamicPoolAllocator(void);

        PoolVector<AllocationUnit<T> >& getDynamicPoolVector(size_t index);
        size_t getNumberOfDynamicPoolVectors() const;

        T** allocateDynamic();
        void deallocateDynamic(T** obj);
        void deallocateDynamicAll();
    };


    template <typename T>
    DynamicPoolAllocator<T>::DynamicPoolAllocator(size_t defaultSize)
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
            T** end = &m_proxy[m_indexProxy][0] + m_proxy[m_indexProxy].size();
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
        return proxy;
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
        if(m_headPool == end)
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
        return proxy;
    }

    template <typename T>
    void DynamicPoolAllocator<T>::deallocateDynamic(T** obj)
    {
        AllocationUnit<T>* unit = reinterpret_cast<AllocationUnit<T>* >((reinterpret_cast<char*>(*obj)) - offsetof(AllocationUnit<T>,data));
        deallocateProxy(obj);
        if(m_headPool == &m_pool[m_indexPool].vector[0])
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
            (*unit->proxy) = &unit->data;
        }
        m_pool[m_indexPool].count -= 1;
    }

    template <typename T>
    PoolVector<AllocationUnit<T> >& DynamicPoolAllocator<T>::getDynamicPoolVector(size_t index)
    {
        //assert(index <= m_indexPool);
        return m_pool.at(index);
    }

    template <typename T>
    size_t DynamicPoolAllocator<T>::getNumberOfDynamicPoolVectors() const
    {
        return m_indexPool + 1;
    }
    template <typename T>
    void DynamicPoolAllocator<T>::deallocateDynamicAll()
    {
        m_pool.resize(0);
        m_proxy.resize(0);
        m_proxyRecycle.resize(0);

        m_indexPool=0;
        AllocationUnit<T>* m_headPool=0;

        size_t m_indexProxy=0;
        T** m_headProxy=0;
    }
}
#endif