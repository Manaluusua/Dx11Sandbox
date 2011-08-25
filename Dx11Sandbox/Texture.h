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
       
        
        ID3D11ShaderResourceView* GetShaderResourceView() { return m_shaderView; }
        ID3D11RenderTargetView* GetRenderTargetView() { return m_rtView; }
        
        void SetShaderResourceView(ID3D11ShaderResourceView* view) { m_shaderView = view; }
        void SetRenderTargetView(ID3D11RenderTargetView* view) { m_rtView = view; }

        PixelBox* readPixelBoxFromTexture(UINT arrayIndex=0, UINT mipSlice=0, UINT mips=1);

    private:


        static Texture* CreateEmptyTexture2D(ID3D11Device* device, const wstring& texname, UINT texWidth, UINT texHeight,
            UINT arraySize, UINT bindFlags, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT, UINT cpuAccess = 0,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

        static Texture* CreateTextureFromFile(ID3D11Device* device, const wstring& filename,const wstring& texname,
            UINT cpuAccess = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);

        Texture(const wstring texname);
        wstring m_name;
        ID3D11ShaderResourceView* m_shaderView;
        ID3D11RenderTargetView* m_rtView;
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