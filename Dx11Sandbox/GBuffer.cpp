#include "GBuffer.h"
#include "Texture.h"
#include "TextureManager.h"
#include "RenderContext.h"

namespace Dx11Sandbox
{
	GBuffer::GBuffer(RenderContext* context)
		:m_bufferWidth(0),
		m_bufferHeight(0),
		m_context(context),
		m_textures(new Texture*[GBUFFER_TEXTURE_COUNT]),
		m_isAllocated(false)
		
	{
	}
	

	GBuffer::~GBuffer()
	{
		SAFE_DELETE_ARRAY(m_textures);
	}

	void GBuffer::allocateBuffer(unsigned int w, unsigned int h)
	{
		if (m_isAllocated && m_bufferWidth == w && m_bufferHeight == h) return;
		m_bufferWidth = w;
		m_bufferHeight = h;
		deallocate();
		allocate();
	}

	void GBuffer::allocate()
	{
		deallocate(); 
		
		Texture* albedo = TextureManager::singleton()->createTexture2D(m_context->getDevice(), "GBUFFER_ALBEDO", m_bufferWidth, m_bufferHeight, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_DEFAULT);
		Texture* normal = TextureManager::singleton()->createTexture2D(m_context->getDevice(), "GBUFFER_NORMAL", m_bufferWidth, m_bufferHeight, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_DEFAULT);
		Texture* specular = TextureManager::singleton()->createTexture2D(m_context->getDevice(), "GBUFFER_SPECULAR", m_bufferWidth, m_bufferHeight, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_USAGE_DEFAULT);
		Texture* depth = TextureManager::singleton()->createTexture2D(m_context->getDevice(), "GBUFFER_SPECULAR", m_bufferWidth, m_bufferHeight, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, DXGI_FORMAT_D32_FLOAT, 0, D3D11_USAGE_DEFAULT);

		m_textures[ALBEDO] = albedo;
		m_textures[NORMAL] = normal;
		m_textures[SPECULAR] = specular;
		m_textures[DEPTH] = depth;

		m_isAllocated = true;
	}
	void GBuffer::deallocate()
	{
		if (!m_isAllocated) return;

		for (unsigned int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i){
			Texture* tex = m_textures[i];
			TextureManager::singleton()->releaseTexture(tex->getName());
		}
		m_isAllocated = false;
	}
}
