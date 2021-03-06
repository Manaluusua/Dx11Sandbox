#ifndef DX11SANDBOX_TEXTUREMANAGER_H
#define DX11SANDBOX_TEXTUREMANAGER_H

#include "Singleton.h"
#include "CommonUtilities.h"
#include <map>
#include <DXUT.h>


namespace Dx11Sandbox
{
    class Texture;
	
    typedef std::map<ResourceID, Texture*>::iterator TextureMapIterator;
	

    class TextureManager : public Singleton<TextureManager>
    {
		SINGLETON(TextureManager)
    public:

        
        virtual ~TextureManager();

        Texture* createTexture(const string& texname);
        Texture* getOrCreateTexture( const string& texname);
       
		Texture* getOrCreateTextureFromFile(ID3D11Device* device, const string& filename, const string& texname, UINT cpuAccess = 0,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT filter = D3DX11_FILTER_POINT | D3DX11_FILTER_SRGB_IN);

		/*
		 Texture* getOrCreateTextureFromFile(ID3D11Device* device, const string& texname, UINT texWidth = 512, UINT texHeight = 512,
            UINT arraySize = 1, UINT bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT, UINT cpuAccess = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
       
        Texture* getOrCreateTexture(ID3D11Device* device, const string& filename,const string& texname, UINT cpuAccess = 0,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT, UINT filter =  D3DX11_FILTER_POINT | D3DX11_FILTER_SRGB_IN);
		*/

        Texture* getTexture(ResourceID id);
        bool releaseTexture(ResourceID id);

		void setAssetPath(const string& path);
		const string& getAssetPath() const;

    private:


        DISABLE_COPY(TextureManager)
        TextureManager();

		string m_assetPath;

        std::map<ResourceID, Texture*> m_loadedTextures;
    };
}
#endif
