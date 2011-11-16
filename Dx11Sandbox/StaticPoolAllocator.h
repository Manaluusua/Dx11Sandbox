#ifndef DX11SANDBOX_STATICPOOLALLOCATOR_H
#define DX11SANDBOX_STATICPOOLALLOCATOR_H

#include <vector>
namespace Dx11Sandbox
{

    template <typename T>
    class StaticPoolAllocator
    {
    private:

        std::vector<std::vector<T> > m_pool;
        size_t m_index;
    public:
        StaticPoolAllocator();
        ~StaticPoolAllocator(void);

        const std::vector<T>* const getStaticPoolVector(size_t index);
        size_t getNumberOfStaticPoolVectors();

        T* allocateStatic(unsigned int numObjects);
        void deallocateStatic();

    };


        template <class T>
    StaticPoolAllocator<T>::StaticPoolAllocator()
        :m_index(0)
    {
    }

    template <class T>
    StaticPoolAllocator<T>::~StaticPoolAllocator()
    {

    }


    template <class T>
    T* StaticPoolAllocator<T>::allocateStatic(unsigned int numObjects)
    {
        T* retVal;
        m_pool.resize(m_pool.size()+1);
        m_pool[m_index].resize(numObjects);
        retVal = &m_pool[m_index][0];
        ++m_index;
        return retVal;
    }
   
    template <class T>
    void StaticPoolAllocator<T>::deallocateStatic()
    {
        m_pool.resize(0);
        m_index = 0;
    }

    template <typename T>
    const std::vector<T>* const StaticPoolAllocator<T>::getStaticPoolVector(size_t index)
    {
        assert(index <= m_index);
        return &m_pool.at(index);
    }

    template <typename T>
    size_t StaticPoolAllocator<T>::getNumberOfStaticPoolVectors()
    {
        return m_index;
    }

}
#endif