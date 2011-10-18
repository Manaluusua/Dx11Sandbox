#include "StaticPoolAllocator.h"
namespace Dx11Sandbox
{
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
        m_pool.resize(m_pool.size()+1);
        
        m_pool[m_index].resize(numObjects);
        retVal = m_pool[m_index][0];
        ++m_index;
        return retVal;
    }
   
    template <class T>
    void deallocateStatic()
    {
        m_pool.resize(0);
        m_index = 0;
    }



}