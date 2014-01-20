#include "TextureManager.h"
#include "Texture.h"



namespace Dx11Sandbox
{

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


    Texture* TextureManager::createTexture2D(ID3D11Device* device, const string& texname, UINT texWidth, UINT texHeight,
        UINT arraySize, UINT bindFlags, DXGI_FORMAT format, UINT cpuAccess, D3D11_USAGE usage)
    {
		ResourceID id = stringToID(texname);

        //names must be UNIQUE
		assert(m_loadedTextures.find(id)==m_loadedTextures.end());
        if (m_loadedTextures.find(id)!=m_loadedTextures.end())
        {
			return 0;
        }

        Texture* tex = Texture::CreateTexture2D(device,id, texWidth, texHeight, arraySize, bindFlags,format,cpuAccess, usage);
        if(tex)
        {
            m_loadedTextures[id] = tex;
            return tex;
        }
        return 0;
    }

    Texture* TextureManager::createTexture(ID3D11Device* device, const string& filename,const string& texname,
        UINT cpuAccess, D3D11_USAGE usage, UINT filter)
    {
		ResourceID id = stringToID(texname);

        //names must be UNIQUE
		assert(m_loadedTextures.find(id)==m_loadedTextures.end());
        if (m_loadedTextures.find(id)!=m_loadedTextures.end())
        {
			return 0;
        }

        Texture* tex = Texture::CreateTextureFromFile(device, filename,id,cpuAccess, usage, filter);
        if(tex)
        {

            m_loadedTextures[id] = tex;
            return tex;
        }
        return 0;
    }

    Texture* TextureManager::getOrCreateTexture(ID3D11Device* device, const string& texname, UINT texWidth , UINT texHeight,
        UINT arraySize, UINT bindFlags, DXGI_FORMAT format, UINT cpuAccess, D3D11_USAGE usage )
    {
        
		ResourceID id = stringToID(texname);

		Texture* tex = getTexture(id);
		if(!tex)
		{
			tex = createTexture2D(device,texname, texWidth, texHeight, arraySize, bindFlags,format, cpuAccess, usage);
		}
   
        return tex;
    }
    Texture* TextureManager::getOrCreateTexture(ID3D11Device* device, const string& filename,const string& texname,
        UINT cpuAccess, D3D11_USAGE usage, UINT filter)
    {
        
		ResourceID id = stringToID(texname);
		Texture* tex = getTexture(id);
		if(!tex)
		{
            tex = createTexture(device,filename, texname, cpuAccess,usage, filter);
        }
        return tex;
    }


    bool TextureManager::releaseTexture(ResourceID id)
    {
        Texture* tex = getTexture(id);
        if(tex)
        {
            SAFE_DELETE(tex);
            m_loadedTextures.erase(id);
            return true;
        }else
        {
            return false;
        }
    }

    Texture* TextureManager::getTexture(ResourceID id)
    {
        if(m_loadedTextures.find(id)!=m_loadedTextures.end())
            return m_loadedTextures.at(id);
        else
            return 0;
    }



    

}
