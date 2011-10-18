#ifndef DX11SANDBOX_DYNAMICPOOLALLOCATOR_H
#define DX11SANDBOX_DYNAMICPOOLALLOCATOR_H

#include <vector>
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
        PoolVector():count(0){}
    public:
        std::vector<T> vector;
        unsigned int count;
    };

    template <typename T>
    class DynamicPoolAllocator
    {
    private:

        std::vector<PoolVector<AllocationUnit<T> > > m_pool;

        std::vector<std::vector<T*> > m_proxy;
        std::vector<T**> m_proxyRecycle;

        unsigned int m_indexPool;
        AllocationUnit<T>* m_headPool;
        unsigned int m_defaultAllocSize;

        unsigned int m_indexProxy;
        AllocationUnit<T>** m_headProxy;

        inline T** getProxy();
        inline void deallocateProxy(T** proxy);

    public:
        DynamicPoolAllocator(unsigned int defaultSize=40);
        ~DynamicPoolAllocator(void);

        T** allocateDynamic();
        void deallocateDynamic(T** obj);

    };
}
#endif