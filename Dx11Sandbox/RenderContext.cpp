#include "RenderContext.h"
//#include <D3D11.h>
#include "Mesh.h"
#include "Material.h"

namespace Dx11Sandbox
{

    RenderContext::RenderContext(void)
        :m_boundMesh(0),
        m_boundMaterial(0)
    {
    }


    RenderContext::~RenderContext(void)
    {
    }

    void RenderContext::bindMesh(Mesh* mesh)
    {
        if(mesh->bind(this))
            m_boundMesh = mesh;
    }
    void RenderContext::bindMaterial(Material* mat)
    {
        if(mat->bind(this))
            m_boundMaterial = mat;
    }
}
