#include "TextureManager.h"
#include "Texture.h"

namespace Dx11Sandbox
{
    TextureManager* TextureManager::m_instance = 0;

    TextureManager::TextureManager()
    {
    }


    TextureManager::~TextureManager()
    {
        TextureMapIterator it = m_loadedTextures.begin();
        while(it != m_loadedTextures.end())
        {
            SAFE_DELETE(it->second);
            ++it;
        }
        m_loadedTextures.clear();
            
    }

    Texture* TextureManager::createTexture2D(ID3D11Device* device, const wstring& texname, UINT texWidth, UINT texHeight,
        UINT arraySize, UINT bindFlags, DXGI_FORMAT format, UINT cpuAccess, D3D11_USAGE usage)
    {
        //names must be UNIQUE
        assert(m_loadedTextures.find(texname)==m_loadedTextures.end());
        if (m_loadedTextures.find(texname)!=m_loadedTextures.end())
        {
            return 0;
        }

        Texture* tex = Texture::CreateEmptyTexture2D(device,texname, texWidth, texHeight, arraySize, bindFlags,format,cpuAccess, usage);
        if(tex)
        {
            m_loadedTextures[texname] = tex;
            return tex;
        }
        return 0;
    }

    Texture* TextureManager::createTexture(ID3D11Device* device, const wstring& filename,const wstring& texname,
        UINT cpuAccess, D3D11_USAGE usage)
    {
        //names must be UNIQUE
        assert(m_loadedTextures.find(texname)==m_loadedTextures.end());
        if (m_loadedTextures.find(texname)!=m_loadedTextures.end())
        {
            return 0;
        }

        Texture* tex = Texture::CreateTextureFromFile(device, filename,texname,cpuAccess, usage);
        if(tex)
        {
            m_loadedTextures[texname] = tex;
            return tex;
        }
        return 0;
    }

    Texture* TextureManager::getOrCreateTexture(ID3D11Device* device, const wstring& texname, UINT texWidth , UINT texHeight,
        UINT arraySize, UINT bindFlags, DXGI_FORMAT format, UINT cpuAccess, D3D11_USAGE usage )
    {
        
        Texture* tex = getTexture(texname);
        if(!tex)
        {
            if(createTexture2D(device,texname, texWidth, texHeight, arraySize, bindFlags,format, cpuAccess, usage))
            {
                tex = getTexture(texname);
            }
        }
        return tex;
    }
    Texture* TextureManager::getOrCreateTexture(ID3D11Device* device, const wstring& filename,const wstring& texname,
        UINT cpuAccess, D3D11_USAGE usage)
    {
        
        Texture* tex = getTexture(texname);
        if(!tex)
        {
            if(createTexture(device,filename, texname, cpuAccess,usage))
            {
                tex = getTexture(texname);
            }
        }
        return tex;
    }


    bool TextureManager::releaseTexture(const wstring& texname)
    {
        Texture* tex = getTexture(texname);
        if(tex)
        {
            SAFE_DELETE(tex);
            m_loadedTextures.erase(texname);
            return true;
        }else
        {
            return false;
        }
    }

    Texture* TextureManager::getTexture(const wstring& texname)
    {
        if(m_loadedTextures.find(texname)!=m_loadedTextures.end())
            return m_loadedTextures.at(texname);
        else
            return 0;
    }

    TextureManager* const TextureManager::getSingleton()
    {
        if(!m_instance)
        {
            m_instance = new TextureManager();
        }
        return m_instance;
    }

    void TextureManager::destroyTextureManager()
    {
        SAFE_DELETE(m_instance);
    }


    

}
