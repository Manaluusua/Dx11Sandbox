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
        
		void createResource(ID3D11Device* device, uint32_t texWidth, uint32_t texHeight, bool createViews = true,
			uint32_t bindFlags = D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT,
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t arraySize = 1, uint32_t cpuAccess = 0);

		void createResourceFromFile(ID3D11Device* device, const string& filename, uint32_t cpuAccess = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, uint32_t filter =0 /*= D3DX11_FILTER_POINT | D3DX11_FILTER_SRGB_IN*/);

		void createShaderResourceView(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, int mipLevels = 1);
		void createRenderTargetView(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
		void createDepthStencilView(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);
		void createUnorderedAccessView(ID3D11Device* device, DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN);

        ID3D11ShaderResourceView* getShaderResourceView();
        ID3D11RenderTargetView* getRenderTargetView();
		ID3D11DepthStencilView* getDepthStencilView();
		ID3D11UnorderedAccessView* getUnorderedAccessView();

        PixelBox* readPixelBoxFromTexture(uint32_t arrayIndex=0, uint32_t mipSlice=0, uint32_t mips=1);

		ResourceID getName() const;

		uint32_t getWidth() const;
		uint32_t getHeight() const;

	protected:
		virtual ~Texture(void);

    private:
		DISABLE_COPY(Texture)
		Texture(ResourceID texname);
		void destroyResourcesAndViews();

        ResourceID m_name;
        ID3D11ShaderResourceView* m_shaderResourceView;
        ID3D11RenderTargetView* m_rtView;
		ID3D11DepthStencilView* m_dsView;
		ID3D11UnorderedAccessView* m_uav;
        ID3D11Resource *m_texture;
        DXGI_FORMAT m_format;
        uint32_t m_cpuAccess;
        D3D11_USAGE m_usage;

        uint32_t m_texWidth;
        uint32_t m_texHeight;
        uint32_t m_texArraySize;
        uint32_t m_flags;
    };
}

#endif