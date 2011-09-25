#include "RenderObject.h"
namespace Dx11Sandbox
{
    RenderObject::RenderObject()
        :m_renderObjectMask(0xFFFFFFFF)
    {
        D3DXMatrixIdentity(&m_world);
    }

    RenderObject::~RenderObject()
    {

    }
}