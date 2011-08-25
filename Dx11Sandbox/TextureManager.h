#ifndef DX11SANDBOX_TEXTUREMANAGER_H
#define DX11SANDBOX_TEXTUREMANAGER_H

#include <map>
#include "CommonUtilities.h"
#include <DXUT.h>

namespace Dx11Sandbox
{
    class Texture;
    typedef std::map<wstring, Texture*>::iterator TextureMapIterator;
    class TextureManager
    {
    public:
        static TextureManager* const getSingleton();
        static void destroyTextureManager();

        TextureManager();
        virtual ~TextureManager();

        bool createTexture2D(ID3D11Device* device, const wstring& texname, UINT texWidth = 512, UINT texHeight = 512,
            UINT arraySize = 1, UINT bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT, UINT cpuAccess = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
        
        bool createTexture2D(ID3D11Device* device, const wstring& filename,const wstring& texname, UINT cpuAccess = 0,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
        
        Texture* getOrCreateTexture(ID3D11Device* device, const wstring& texname, UINT texWidth = 512, UINT texHeight = 512,
            UINT arraySize = 1, UINT bindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT, UINT cpuAccess = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
       
        Texture* getOrCreateTexture(ID3D11Device* device, const wstring& filename,const wstring& texname, UINT cpuAccess = 0,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

        Texture* getTexture(const wstring& texname);
        bool releaseTexture(const wstring& texname);

    private:
        static TextureManager* m_instance;
        std::map<wstring, Texture*> m_loadedTextures;
    };
}
#endif
