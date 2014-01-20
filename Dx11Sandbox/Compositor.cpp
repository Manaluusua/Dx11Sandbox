#include "Compositor.h"
#include "TextureManager.h"
#include "Texture.h"

namespace Dx11Sandbox
{

    Compositor::Compositor()
        :m_initialized(false)
    {
    }

    void Compositor::initialize(ID3D11Device* device, int texWidth, int texHeight)
    {
        if(m_initialized)
            return;
		TextureManager *mngr = TextureManager::singleton();


        m_textures[0] = mngr->createTexture2D(device, "CompositorTex1",texWidth, texHeight,1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_USAGE_DEFAULT);
        m_textures[1] = mngr->createTexture2D(device, "CompositorTex2",texWidth, texHeight,1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_USAGE_DEFAULT);

        m_initialized = true;
    }


    Compositor::~Compositor()
    {
        TextureManager *mngr = TextureManager::singleton();
		if(!m_initialized) return;
		mngr->releaseTexture(m_textures[0]->getName());
        mngr->releaseTexture(m_textures[1]->getName());
    }

    void Compositor::renderFullscreenQuad(Effect* effect)
    {

    }

    void Compositor::addCompositorEffect(CompositorEffect* effect, string& name)
    {

    }
    void Compositor::removeCompositorEffect(string& name)
    {

    }

    void Compositor::addEffectToChain(string& name,int index)
    {

    }
    void Compositor::removeEffectFromChain(string& name)
    {

    }

    void Compositor::update(float time, float dt)
    {

    }



}