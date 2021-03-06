#include "GBuffer.h"
#include "Texture.h"
#include "RenderContext.h"

#include "TextureManager.h"
#include "TextureCache.h"

namespace Dx11Sandbox
{

	const unsigned int GBuffer::GBUFFER_TEXTURE_COUNT = 4;

	GBuffer::GBuffer(RenderContext* context)
		:m_bufferWidth(0),
		m_bufferHeight(0),
		m_context(context),
		m_textures(new Texture*[GBUFFER_TEXTURE_COUNT]),
		m_renderTargets(new ID3D11RenderTargetView*[GBUFFER_TEXTURE_COUNT]),
		m_isAllocated(false)
		
	{
	}
	

	GBuffer::~GBuffer()
	{
		deallocate();
		SAFE_DELETE_ARRAY(m_textures);
		SAFE_DELETE_ARRAY(m_renderTargets);
	}

	void GBuffer::setAsRenderTargets()
	{
		if (!m_isAllocated) return;
		
		m_context->pushRenderTargets(GBUFFER_TEXTURE_COUNT, m_renderTargets, m_context->getDefaultDepthStencilTexture()->getDepthStencilView());
		
	}

	void GBuffer::clear()
	{
		if (!m_isAllocated) return;
		float clearColor[4] = { 0.f, 0.f, 0.f, 0.0f };
		
		for (unsigned int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i){
			m_context->getImmediateContext()->ClearRenderTargetView(m_textures[i]->getRenderTargetView(), clearColor);
		}

		
		m_context->getImmediateContext()->ClearDepthStencilView(m_context->getDefaultDepthStencilTexture()->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0, 0);
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

		
		
		Texture* albedo = TextureManager::singleton()->createTexture("GBUFFER_ALBEDO");
		albedo->createResource(m_context->getDevice(), m_bufferWidth, m_bufferHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

		Texture* normal = TextureManager::singleton()->createTexture("GBUFFER_NORMAL");
		normal->createResource(m_context->getDevice(), m_bufferWidth, m_bufferHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

		Texture* specular = TextureManager::singleton()->createTexture("GBUFFER_SPECULAR");
		specular->createResource(m_context->getDevice(), m_bufferWidth, m_bufferHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

		Texture* refl = TextureManager::singleton()->createTexture("GBUFFER_ENVIRONMENT");
		refl->createResource(m_context->getDevice(), m_bufferWidth, m_bufferHeight, true, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT);

		Texture* depth = m_context->getDefaultDepthStencilTexture();

		m_textures[ALBEDO] = albedo;
		m_textures[NORMAL] = normal;
		m_textures[SPECULAR] = specular;
		m_textures[ENVIRONMENT] = refl;

		m_renderTargets[ALBEDO] = albedo->getRenderTargetView();
		m_renderTargets[NORMAL] = normal->getRenderTargetView();
		m_renderTargets[SPECULAR] = specular->getRenderTargetView();
		m_renderTargets[ENVIRONMENT] = refl->getRenderTargetView();

		//add to cache
		TextureCache* cache = TextureCache::singleton();
		cache->setCachedTextures(TextureCache::GALBEDO, &albedo, 1);
		cache->setCachedTextures(TextureCache::GNORMAL, &normal, 1);
		cache->setCachedTextures(TextureCache::GSPECULAR, &specular, 1);
		cache->setCachedTextures(TextureCache::GENVIRONMENT, &refl, 1);
		cache->setCachedTextures(TextureCache::SCENEDEPTH, &depth, 1);
		
		m_isAllocated = true;
	}

	unsigned int GBuffer::getWidth() const
	{
		return m_bufferWidth;

	}
	unsigned int GBuffer::getHeight() const
	{
		return m_bufferHeight;
	}

	Texture* GBuffer::getBufferTexture(GBufferTexture texType)
	{
		return m_textures[texType];

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
