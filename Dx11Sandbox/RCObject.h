#ifndef DX11SANDBOX_RCOBJECT_H
#define DX11SANDBOX_RCOBJECT_H

#include "CommonUtilities.h"

namespace Dx11Sandbox
{

    class RCObject
    {
    public:
        RCObject(void);
        virtual ~RCObject(void);

        void incrementRef();
        void decrementRef();

    private:
        DISABLE_COPY(RCObject)

        size_t m_referenceCount;
    };

    inline void RCObject::incrementRef()
    {
        ++m_referenceCount;
    }
    inline void RCObject::decrementRef()
    {
        if( --m_referenceCount == 0 )
        {
            delete this;
        }
    }

}

#endif