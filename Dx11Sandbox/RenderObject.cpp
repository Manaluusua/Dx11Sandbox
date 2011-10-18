#include "RenderObject.h"
namespace Dx11Sandbox
{
    RenderObject::RenderObject()
        :mesh(0),
        mat(0),
        renderObjectMask(0xFFFFFFFF),
        renderQueueFlag(RDEFAULT)
    {

    }

    RenderObject::~RenderObject()
    {

    }
}