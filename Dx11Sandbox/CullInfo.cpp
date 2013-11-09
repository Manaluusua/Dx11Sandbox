#include "CullInfo.h"
namespace Dx11Sandbox
{
    CullInfo::CullInfo()
        :boundingSphere(-1,-1,-1,-1),
        object(0),
        renderMask( RENDERMASK_DEFAULT_OPAQUE )
    {

    }

    CullInfo::~CullInfo()
    {

    }
}