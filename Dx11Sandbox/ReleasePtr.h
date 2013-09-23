#ifndef DX11SANDBOX_ReleasePtr_H
#define DX11SANDBOX_ReleasePtr_H

namespace Dx11Sandbox
{
    /* this ptr will only release resources, not increment the ref, used usually for automatic release of some DXResources. */
    template< class T >
    class ReleasePtr
    {
    public:

        ReleasePtr();

        ReleasePtr( T* raw  );
        
        ReleasePtr( const ReleasePtr<T>& other );

        const ReleasePtr<T>& operator=( const ReleasePtr<T>& other );

        //implicit
        operator T*();

        //explicit
        T* rawPtr();

        T& operator*() const;

        T* operator->() const;

        ~ReleasePtr();

    private:
        mutable bool m_release;
        T* m_rawPtr;
    };


    template< class T >
    inline ReleasePtr<T>::ReleasePtr()
        :m_release( true ),
        m_rawPtr( 0 )
    {

    }

    template< class T >
    inline ReleasePtr<T>::ReleasePtr( T* raw  )
        :m_release( true ),
        m_rawPtr( raw )
    {
    }

    template< class T >
    inline ReleasePtr<T>::ReleasePtr( const ReleasePtr<T>& other )
        :m_release( true ),
        m_rawPtr( other.m_rawPtr )
    {
        other.m_release = false;
    }

    template< class T >
    inline ReleasePtr<T>::~ReleasePtr()
    {
        if(m_rawPtr && m_release)
        {
            m_rawPtr->Release();
        }
    }

    template< class T >
    inline const ReleasePtr<T>& ReleasePtr<T>::operator=( const ReleasePtr<T>& other )
    {
        if( m_rawPtr != other.m_rawPtr )
        {
            if( m_rawPtr && m_release )
            {
                m_rawPtr->Release();
            }
            m_rawPtr = other.m_rawPtr;
            m_release = true;
            
            other.m_release = false;
        }

        return *this;
    }

    template< class T >
    inline ReleasePtr<T>::operator T*()
    {
        return m_rawPtr;
    }

    

    template< class T >
    inline T* ReleasePtr<T>::rawPtr()
    {
        return m_rawPtr;
    }

    template< class T >
    inline T& ReleasePtr<T>::operator*() const
    {
        return *m_rawPtr;
    }

    template< class T >
    inline T* ReleasePtr<T>::operator->() const
    {
        return m_rawPtr;
    }
    


    

}

#endif