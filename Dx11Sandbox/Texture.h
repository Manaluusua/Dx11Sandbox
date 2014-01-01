#ifndef DX11SANDBOX_TEXTURE_H
#define DX11SANDBOX_TEXTURE_H
#include "CommonUtilities.h"
#include "DXUT.h"

struct ID3D11Device;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11Resource;

namespace Dx11Sandbox
{
    class PixelBox;

    class Texture
    {
        friend class TextureManager;
    public:
        virtual ~Texture(void);
       
        
        ID3D11ShaderResourceView* GetShaderResourceView();
        ID3D11RenderTargetView* GetRenderTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();

        PixelBox* readPixelBoxFromTexture(UINT arrayIndex=0, UINT mipSlice=0, UINT mips=1);

    private:
        DISABLE_COPY(Texture)

        static Texture* CreateTexture2D(ID3D11Device* device, const string& texname, UINT texWidth, UINT texHeight,
            UINT arraySize, UINT bindFlags, DXGI_FORMAT format , UINT cpuAccess,
            D3D11_USAGE usage );

        static Texture* CreateTextureFromFile(ID3D11Device* device, const string& filename,const string& texname,
            UINT cpuAccess , D3D11_USAGE usageT, UINT filter );

        Texture(const string texname);
        string m_name;
        ID3D11ShaderResourceView* m_shaderView;
        ID3D11RenderTargetView* m_rtView;
		ID3D11DepthStencilView* m_dsView;
        ID3D11Resource *m_texture;
        DXGI_FORMAT m_format;
        UINT m_cpuAccess;
        D3D11_USAGE m_usage;

        UINT m_texWidth;
        UINT m_texHeight;
        UINT m_texArraySize;
        UINT m_flags;
    };
}

#endif