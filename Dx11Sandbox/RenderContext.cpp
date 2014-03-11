#include "RenderContext.h"
//
#include "Mesh.h"
#include "Material.h"
#include "DXUT.h"
#include "EnvironmentInfo.h"
#include "TextureManager.h"
#include "Texture.h"
namespace Dx11Sandbox
{

    RenderContext::RenderContext(void)
		:m_customClipPlane(0, 0, 0, 0),
		m_boundMesh(0),
        m_boundMaterial(0),
		m_defaultDepthStencil(0)

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


	void RenderContext::createDepthStencil(int w, int h)
	{
		TextureManager* texMngr = TextureManager::singleton();
		if (m_defaultDepthStencil){
			texMngr->releaseTexture(m_defaultDepthStencil->getName());
		}

		m_defaultDepthStencil = TextureManager::singleton()->createTexture("DepthStencil");
		m_defaultDepthStencil->createResource(getDevice(), w, h, false, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32_TYPELESS);
		m_defaultDepthStencil->createDepthStencilView(getDevice(), DXGI_FORMAT_D32_FLOAT);
		m_defaultDepthStencil->createShaderResourceView(getDevice(), DXGI_FORMAT_R32_FLOAT);

	}

	ID3D11Device* RenderContext::getDevice(){ return m_device; }
	void RenderContext::setDevice(ID3D11Device* device){ m_device = device; }

	ID3D11DeviceContext* RenderContext::getImmediateContext(){ return m_imContext; }
	void RenderContext::setImmediateContext(ID3D11DeviceContext* imContext){ m_imContext = imContext; }

	Mesh* RenderContext::getBoundMesh(){ return m_boundMesh; }
	Material* RenderContext::getBoundMaterial(){ return m_boundMaterial; }


	void RenderContext::setCustomClipPlane(D3DXVECTOR4& plane){ m_customClipPlane = plane; }
	const D3DXVECTOR4& RenderContext::getCustomClipPlane(){ return m_customClipPlane; }

	Texture* RenderContext::getDefaultDepthStencilTexture()
	{
		return m_defaultDepthStencil;
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

		createDepthStencil(m_defaultViewport.Width, m_defaultViewport.Height);

	}

	void RenderContext::disableDepthStencil(bool val)
	{
		bindCurrentRenderTargetState(!val);
	}

	void RenderContext::bindCurrentRenderTargetState(bool bindDepthStencil)
	{
		
		if(m_boundStates.size() == 0u)
		{
			ID3D11RenderTargetView * backBuffer[1];
			backBuffer[0] = DXUTGetD3D11RenderTargetView();
			m_imContext->OMSetRenderTargets(1, backBuffer, bindDepthStencil ? m_defaultDepthStencil->getDepthStencilView() : 0);


		}else{
			const RenderTargetsState& currentState = m_boundStates.back();
			m_imContext->OMSetRenderTargets(currentState.numberOfBoundTargets, currentState.renderTargets, bindDepthStencil ? currentState.depthStencil : 0);

		}
		
	}

}
