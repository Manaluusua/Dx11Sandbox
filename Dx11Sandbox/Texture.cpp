#include "Texture.h"
#include "SDKmisc.h"
#include "PixelBox.h"
#include <memory>
namespace Dx11Sandbox
{
    Texture::Texture(ResourceID texname)
        :m_name(texname),
		m_shaderResourceView(0),
        m_rtView(0),
		m_dsView(0),
		m_uav(0),
        m_texture(0),
        m_texWidth(0),
        m_texHeight(0),
        m_texArraySize(0),
        m_format(DXGI_FORMAT_UNKNOWN),
        m_cpuAccess(0),
        m_usage(D3D11_USAGE_DEFAULT)
    {
       
    }


    Texture::~Texture(void)
    {
		destroyResourcesAndViews();
    }

	void Texture::destroyResourcesAndViews()
	{
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_shaderResourceView);
		SAFE_RELEASE(m_rtView);
		SAFE_RELEASE(m_dsView);
		SAFE_RELEASE(m_uav);

	}
	void Texture::createResource(ID3D11Device* device, UINT texWidth, UINT texHeight, bool createViews,
		UINT bindFlags, DXGI_FORMAT format,D3D11_USAGE usage, UINT arraySize, UINT cpuAccess)
	{

		destroyResourcesAndViews();

		m_texArraySize = arraySize;
		m_texWidth = texWidth;
		m_texHeight = texHeight;
		m_flags = bindFlags;
		m_format = format;
		m_cpuAccess = cpuAccess;
		m_usage = usage;

		D3D11_TEXTURE2D_DESC desc;
		memset(&desc, 0, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = texWidth;
		desc.Height = texHeight;
		desc.MipLevels = 1;
		desc.ArraySize = arraySize;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.CPUAccessFlags = cpuAccess;
		desc.Usage = usage;
		desc.BindFlags = bindFlags;
		desc.MiscFlags = 0x0;

		ID3D11Texture2D *pEmptyTex = NULL;
		if (FAILED(device->CreateTexture2D(&desc, NULL, &pEmptyTex)))
		{
			return;
		}

		m_texture = pEmptyTex;

		if (!createViews) return;

		if (m_flags & D3D11_BIND_RENDER_TARGET)
		{
			createRenderTargetView(device);
		}
		else if (m_flags & D3D11_BIND_DEPTH_STENCIL)
		{
			createDepthStencilView(device);

		}

		if (m_flags & D3D11_BIND_UNORDERED_ACCESS){
			createUnorderedAccessView(device);
		}


		if (m_flags & D3D11_BIND_SHADER_RESOURCE)
		{
			createShaderResourceView(device);
		}

	}


	void Texture::createShaderResourceView(ID3D11Device* device, DXGI_FORMAT format, int mipLevels)
	{
		if (!(m_flags & D3D11_BIND_SHADER_RESOURCE) || !m_texture) return;
		
		SAFE_RELEASE(m_shaderResourceView);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		memset(&srvDesc, 0, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = format == DXGI_FORMAT_UNKNOWN ? m_format : format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = mipLevels;
		srvDesc.Texture2D.MostDetailedMip = mipLevels - 1;


		device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
		
	}

	void Texture::createRenderTargetView(ID3D11Device* device, DXGI_FORMAT format)
	{
		if (!(m_flags & D3D11_BIND_RENDER_TARGET) || !m_texture) return;
		
		SAFE_RELEASE(m_rtView);

		D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
		memset(&rtDesc, 0, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rtDesc.Format = format == DXGI_FORMAT_UNKNOWN ? m_format : format;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Texture2D.MipSlice = 0;

		device->CreateRenderTargetView(m_texture, &rtDesc, &m_rtView);
		
	}
	void Texture::createDepthStencilView(ID3D11Device* device, DXGI_FORMAT format)
	{
		if (!(m_flags & D3D11_BIND_DEPTH_STENCIL) || !m_texture) return;

		SAFE_RELEASE(m_dsView);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
		memset(&dsDesc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		dsDesc.Format = format == DXGI_FORMAT_UNKNOWN ? m_format : format;
		dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsDesc.Texture2D.MipSlice = 0;
		device->CreateDepthStencilView(m_texture, &dsDesc, &m_dsView);

		
	}
	void Texture::createUnorderedAccessView(ID3D11Device* device, DXGI_FORMAT format)
	{
		if (!(m_flags & D3D11_BIND_UNORDERED_ACCESS) || !m_texture ) return;
		
		SAFE_RELEASE(m_uav);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		uavDesc.Format = format == DXGI_FORMAT_UNKNOWN ? m_format : format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		device->CreateUnorderedAccessView(m_texture, &uavDesc, &m_uav);
	
	}

    
	void Texture::createResourceFromFile(ID3D11Device* device, const string& filepath, UINT cpuAccess, D3D11_USAGE usage, UINT filter)
    {
		destroyResourcesAndViews();
        D3DX11_IMAGE_LOAD_INFO info;
        info.CpuAccessFlags = cpuAccess;
        info.Usage = usage;
        info.Filter = filter ;
        m_cpuAccess = cpuAccess;
        m_usage = usage;


		std::unique_ptr<WCHAR> filePathWide( multibyteStringToWide(filepath) );


        if(usage!=D3D11_USAGE_STAGING)
        {

			if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, filePathWide.get(), &info, NULL, &m_shaderResourceView, NULL)))
            {
				std::string err = std::string("Failed to create resource: ") + filepath;
				showErrorDialog(err.c_str());
                return;
            }
			m_shaderResourceView->GetResource(&m_texture);
        }else
        {
            info.BindFlags = 0;
            if(FAILED(D3DX11CreateTextureFromFile(device, filePathWide.get(), &info, 0,&m_texture, 0)))
            {
                std::string err = std::string("Failed to create resource from file: ") + filepath;
				showErrorDialog(err.c_str());
                return;
            }
        }
        
        

        D3D11_RESOURCE_DIMENSION type;
        m_texture->GetType(&type);
        switch( type )
        {
            
            case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
            {
                D3D11_TEXTURE2D_DESC desc;
                ID3D11Texture2D *pTexture2D = (ID3D11Texture2D*)m_texture;
                pTexture2D->GetDesc( &desc );
                m_texWidth = desc.Width;
                m_texHeight = desc.Height;
                m_texArraySize = desc.ArraySize;
                m_flags = desc.BindFlags;
                m_format = desc.Format;
            }
            break;
            
            default:
            // we do not atm support other types of textures
            showErrorDialog("Tried to create a texture from file containing unsupported texture type");
            break;
        }
    }

	UINT Texture::getWidth() const
	{
		return m_texWidth;
	}
	UINT Texture::getHeight() const
	{
		return m_texHeight;
	}

	ResourceID Texture::getName() const
	{
		return m_name;
	}

	ID3D11ShaderResourceView* Texture::getShaderResourceView() 
	{ 
		return m_shaderResourceView;
	}

	ID3D11RenderTargetView* Texture::getRenderTargetView() 
	{ 
		return m_rtView; 
	}

	ID3D11DepthStencilView* Texture::getDepthStencilView()
	{
		return m_dsView;
	}
	ID3D11UnorderedAccessView*  Texture::getUnorderedAccessView()
	{
		return m_uav;
	}

    PixelBox* Texture::readPixelBoxFromTexture(UINT arrayIndex, UINT mipSlice, UINT mips)
    {

        PixelBox* pixBox;


        if(!(m_format & DXGI_FORMAT_R8G8B8A8_TYPELESS))
        {
            showErrorDialog("Incompatible format, only 32bit float supported atm");
            return 0;
        }

        if(m_usage != D3D11_USAGE_STAGING)
        {
            showErrorDialog("Incorrect usage flags used on create of this texture. Must be D3D11_USAGE_STAGING");
            return 0;
        }

        if(!(m_cpuAccess & D3D11_CPU_ACCESS_READ))
        {
            showErrorDialog("This texture does not have CPU access flag set");
            return 0;
        }

        D3D11_RESOURCE_DIMENSION type;
        m_texture->GetType(&type);
        switch( type )
        {
            
            case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
            {
                ID3D11Device* device;
                ID3D11DeviceContext* context;
                D3D11_MAPPED_SUBRESOURCE mappedTex;


                pixBox = new PixelBox;
                pixBox->allocatePixelBox(m_texWidth, m_texHeight);
                
                //change if need to use deferred context
                m_texture->GetDevice(&device);
                device->GetImmediateContext(&context);
                context->Map(m_texture ,D3D11CalcSubresource(mipSlice, arrayIndex, mips), D3D11_MAP_READ, 0, &mappedTex );

                UCHAR* texels = (UCHAR*)mappedTex.pData;

                PixelBox::Pixel* pixels = pixBox->getPixels();
                for( UINT row = 0; row < m_texHeight; row++ )
                {
                    for( UINT col = 0; col < m_texWidth; col++ )
                    {

                        pixels[row*m_texWidth + col].r =  texels[row*mappedTex.RowPitch + col*4 + 0]; 
                        pixels[row*m_texWidth + col].g =  texels[row*mappedTex.RowPitch + col*4 + 1]; 
                        pixels[row*m_texWidth + col].b =  texels[row*mappedTex.RowPitch + col*4 + 2]; 
                        pixels[row*m_texWidth + col].a =  texels[row*mappedTex.RowPitch + col*4 + 3]; 
                    }
                }
               
                context->Unmap( m_texture,D3D11CalcSubresource(mipSlice, arrayIndex, mips) );
                context->Release();
                device->Release();

            }
            break;
            
            default:
            // we do not atm support other types of textures
            showErrorDialog("Tried to blit a texture that is not either 2D texture or is an array or is a cube tex. Not supported atm");
            break;
        }

       return pixBox;

    }
}