#include "CullInfo.h"
namespace Dx11Sandbox
{

    const CullInfo::CULLINFO_FLAGS CullInfo::CULLINFO_VISIBLE = 0x1;

    CullInfo::CullInfo()
        :boundingSphere(-1,-1,-1,-1),
        mesh(0),
        mat(0),
        binIDFlag( 0 ),
        flags( CULLINFO_VISIBLE )
    {

    }

    CullInfo::~CullInfo()
    {

    }
}