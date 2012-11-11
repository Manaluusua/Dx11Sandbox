#ifndef DX11SANDBOX_RCRELEASEPTR_H
#define DX11SANDBOX_RCRELEASEPTR_H

namespace Dx11Sandbox
{
    /* this ptr will only release resources, not increment the ref, used usually for automatic release of some DXResources. */
    template< class T >
    class RCReleasePtr
    {
    public:

        RCReleasePtr();

        RCReleasePtr( T* raw  );
        
        RCReleasePtr( const RCReleasePtr<T>& other );

        const RCReleasePtr<T>& operator=( const RCReleasePtr<T>& other );

        //implicit
        operator T*();

        //explicit
        T* rawPtr();

        T& operator*() const;

        T* operator->() const;

        ~RCReleasePtr();

    private:
        bool m_release;
        T* m_rawPtr;
    };


    template< class T >
    inline RCReleasePtr<T>::RCReleasePtr()
        :m_release( true ),
        m_rawPtr( 0 )
    {

    }

    template< class T >
    inline RCReleasePtr<T>::RCReleasePtr( T* raw  )
        :m_release( true ),
        m_rawPtr( raw )
    {
    }

    template< class T >
    inline RCReleasePtr<T>::RCReleasePtr( const RCReleasePtr<T>& other )
        :m_release( true ),
        m_rawPtr( other.m_rawPtr )
    {
        other.m_release = false;
    }

    template< class T >
    inline RCReleasePtr<T>::~RCReleasePtr()
    {
        if(m_rawPtr && m_release)
        {
            m_rawPtr->Release();
        }
    }

    template< class T >
    inline const RCReleasePtr<T>& RCReleasePtr<T>::operator=( const RCReleasePtr<T>& other )
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
    inline RCReleasePtr<T>::operator T*()
    {
        return m_rawPtr;
    }

    

    template< class T >
    inline T* RCReleasePtr<T>::rawPtr()
    {
        return m_rawPtr;
    }

    template< class T >
    inline T& RCReleasePtr<T>::operator*() const
    {
        return *m_rawPtr;
    }

    template< class T >
    inline T* RCReleasePtr<T>::operator->() const
    {
        return m_rawPtr;
    }
    


    

}

#endif