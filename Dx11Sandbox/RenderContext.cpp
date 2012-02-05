#include "RenderContext.h"
//
#include "Mesh.h"
#include "Material.h"

namespace Dx11Sandbox
{

    RenderContext::RenderContext(void)
        :m_customFlags(0x0),
        m_boundMesh(0),
        m_boundMaterial(0),
        m_customClipPlane(0,0,0,0),
        m_renderPassID(0)

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

    void RenderContext::clearState()
    {
        m_boundMesh = 0;
        m_boundMaterial = 0;
    }
    void RenderContext::bindMaterial(Material* mat)
    {


        if(mat->bind(this))
            m_boundMaterial = mat;
    }


    void RenderContext::bindRenderTargets(UINT num, ID3D11RenderTargetView *const *renderTargetViews, ID3D11DepthStencilView *depthStencilView)
    {
        
        m_imContext->OMSetRenderTargets(num, renderTargetViews, depthStencilView);
    }


        
    void RenderContext::bindBackBuffer()
    {

        ID3D11RenderTargetView * backBuffer[1];
        backBuffer[0] = DXUTGetD3D11RenderTargetView();
        m_imContext->OMSetRenderTargets(1, backBuffer, DXUTGetD3D11DepthStencilView());

         
    }
}
