#include "RenderContext.h"
//
#include "Mesh.h"
#include "Material.h"

namespace Dx11Sandbox
{

    RenderContext::RenderContext(void)
        :m_boundMesh(0),
        m_boundMaterial(0),
        m_customClipPlane(0,0,0,0)

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



    void RenderContext::pushRenderTargets(UINT num, ID3D11RenderTargetView *const *renderTargetViews, ID3D11DepthStencilView *depthStencilView)
    {
        
		RenderTargetsState currentState;


		currentState.numberOfBoundTargets = num;
		currentState.depthStencil = depthStencilView;

		for(int i = 0; i < num; ++i)
		{
			currentState.renderTargets[i] = renderTargetViews[i];
		}
		
		m_boundStates.push_back(currentState);

        bindCurrentState();
    }

	void RenderContext::popRenderTargets()
	{
		m_boundStates.pop_back();
		bindCurrentState();
	}

    void RenderContext::bindCurrentState()
	{
		if(m_boundStates.size() == 0u)
		{
			ID3D11RenderTargetView * backBuffer[1];
			backBuffer[0] = DXUTGetD3D11RenderTargetView();
			m_imContext->OMSetRenderTargets(1, backBuffer, DXUTGetD3D11DepthStencilView());
		}else{
			const RenderTargetsState& currentState = m_boundStates.back();
			m_imContext->OMSetRenderTargets(currentState.numberOfBoundTargets, currentState.renderTargets, currentState.depthStencil);
		}

		
	}

}
