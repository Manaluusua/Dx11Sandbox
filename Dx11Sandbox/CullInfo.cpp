#include "CullInfo.h"
namespace Dx11Sandbox
{
    CullInfo::CullInfo()
        :boundingSphere(-1,-1,-1,-1),
        mesh(0),
        mat(0),
        renderQueueFlag(RDEFAULT)
    {

    }

    CullInfo::~CullInfo()
    {

    }
}