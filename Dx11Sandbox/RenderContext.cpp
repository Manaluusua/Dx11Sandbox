#include "RenderContext.h"
//
#include "Mesh.h"
#include "Material.h"
#include "EnvironmentInfo.h"

namespace Dx11Sandbox
{

    RenderContext::RenderContext(void)
        :m_boundMesh(0),
        m_boundMaterial(0),
        m_customClipPlane(0,0,0,0)

    {
		memset(&m_defaultViewport, 0, sizeof(D3D11_VIEWPORT));
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

        bindCurrentRenderTargetState();
    }

	void RenderContext::popRenderTargets()
	{
		m_boundStates.pop_back();
		bindCurrentRenderTargetState();
	}

	void RenderContext::pushViewports(UINT viewportCount, D3D11_VIEWPORT* viewports)
	{
		ViewportState state;

		state.viewports = viewports;
		state.viewportCount = viewportCount;
		m_viewPortStates.push_back(state);
		bindCurrentViewports();
	}
	void RenderContext::popViewports()
	{
		if(m_viewPortStates.size() == 0) return;
		m_viewPortStates.pop_back();
		bindCurrentViewports();
	}

	void RenderContext::bindCurrentViewports()
	{
		if(m_viewPortStates.size() == 0u)
		{
			
			m_imContext->RSSetViewports(1, &m_defaultViewport);
		}else{
			ViewportState& vpDef =  m_viewPortStates.back();
			m_imContext->RSSetViewports(vpDef.viewportCount, vpDef.viewports);

		}	
	}

	void RenderContext::setDefaultViewport(D3D11_VIEWPORT* viewport)
	{
		m_defaultViewport.Height = viewport->Height;
		m_defaultViewport.MaxDepth = viewport->MaxDepth;
		m_defaultViewport.MinDepth = viewport->MinDepth;
		m_defaultViewport.TopLeftX = viewport->TopLeftX;
		m_defaultViewport.TopLeftY = viewport->TopLeftY;
		m_defaultViewport.Width = viewport->Width;
	}

    void RenderContext::bindCurrentRenderTargetState()
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
