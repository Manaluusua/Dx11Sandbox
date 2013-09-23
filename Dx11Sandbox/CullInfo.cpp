#include "CullInfo.h"
namespace Dx11Sandbox
{


    CullInfo::CullInfo()
        :boundingSphere(-1,-1,-1,-1),
        object(0),
        binIDFlag( 0 ),
        flags( 0xFFFFFFFF )
    {

    }

    CullInfo::~CullInfo()
    {

    }
}