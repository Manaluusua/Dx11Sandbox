#ifndef DX11SANDBOX_ReleasePtr_H
#define DX11SANDBOX_ReleasePtr_H

namespace Dx11Sandbox
{
    
    template< class T >
    class ReleasePtr
    {
    public:

        ReleasePtr();

        ReleasePtr( T* raw, bool increaseRef = true  );
        
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

        T* m_rawPtr;
    };


    template< class T >
    inline ReleasePtr<T>::ReleasePtr()
        :m_rawPtr( 0 )
    {

    }

    template< class T >
    inline ReleasePtr<T>::ReleasePtr( T* raw , bool increaseRef )
        :m_rawPtr( raw )
    {
		if(m_rawPtr && increaseRef)
		{
			m_rawPtr->AddRef();
		}
		
    }

    template< class T >
    inline ReleasePtr<T>::ReleasePtr( const ReleasePtr<T>& other )
        :m_rawPtr( other.m_rawPtr )
    {
        if(m_rawPtr)
		{
			m_rawPtr->AddRef();
		}
    }

    template< class T >
    inline ReleasePtr<T>::~ReleasePtr()
    {
        if(m_rawPtr)
        {
            m_rawPtr->Release();
        }
    }

    template< class T >
    inline const ReleasePtr<T>& ReleasePtr<T>::operator=( const ReleasePtr<T>& other )
    {
        if( m_rawPtr != other.m_rawPtr )
        {
            if( m_rawPtr)
            {
                m_rawPtr->Release();
            }
            m_rawPtr = other.m_rawPtr;
            
			if(m_rawPtr)
			{
				m_rawPtr->AddRef();
			}
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