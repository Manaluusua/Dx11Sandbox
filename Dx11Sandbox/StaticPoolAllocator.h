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
        unsigned long m_index;
    public:
        StaticPoolAllocator();
        ~StaticPoolAllocator(void);

        T* allocateStatic(unsigned int numObjects);
        void deallocateStatic();

    };
}
#endif