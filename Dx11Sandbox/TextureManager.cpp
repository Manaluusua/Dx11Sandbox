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


   
    Texture* TextureManager::createTexture(const string& texname)
    {
		ResourceID id = stringToID(texname);

        //names must be UNIQUE
		assert(m_loadedTextures.find(id)==m_loadedTextures.end());
        if (m_loadedTextures.find(id)!=m_loadedTextures.end())
        {
			return 0;
        }


        Texture* tex = new Texture(id);
        if(tex)
        {

            m_loadedTextures[id] = tex;
            return tex;
        }
        return 0;
    }

	Texture* TextureManager::getOrCreateTextureFromFile(ID3D11Device* device, const string& filename, const string& texname, uint32_t cpuAccess,
		D3D11_USAGE usage, uint32_t filter)
	{
		ResourceID id = stringToID(texname);
		Texture* tex = getTexture(id);
		if (!tex)
		{
			tex = createTexture(texname);
			tex->createResourceFromFile(device, getAssetPath() + filename, cpuAccess, usage, filter);
		}
		return tex;
	}

    Texture* TextureManager::getOrCreateTexture( const string& texname )
    {
        
		ResourceID id = stringToID(texname);

		Texture* tex = getTexture(id);
		if(!tex)
		{
			tex = createTexture(texname);
		}
   
        return tex;
    }
    
	void TextureManager::setAssetPath(const string& path)
	{
		m_assetPath = path;
	}

	const string& TextureManager::getAssetPath() const
	{
		return m_assetPath;
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
