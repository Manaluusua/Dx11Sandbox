#include "Texture.h"
#include "SDKmisc.h"
#include "PixelBox.h"
#include <memory>
namespace Dx11Sandbox
{
    Texture::Texture(ResourceID texname)
        :m_name(texname),
        m_shaderView(0),
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
        SAFE_RELEASE(m_texture);
        SAFE_RELEASE(m_shaderView);
        SAFE_RELEASE(m_rtView);
		SAFE_RELEASE(m_dsView);
		SAFE_RELEASE(m_uav);
    }

    Texture* Texture::CreateTexture2D(ID3D11Device* device, ResourceID texname, UINT texWidth, UINT texHeight,
        UINT arraySize, UINT bindFlags, DXGI_FORMAT format, UINT cpuAccess, D3D11_USAGE usage)
    {
        Texture* tex = new Texture(texname);
        tex->m_texArraySize = arraySize;
        tex->m_texWidth = texWidth;
        tex->m_texHeight = texHeight;
        tex->m_flags = bindFlags;
        tex->m_format = format;
        tex->m_cpuAccess = cpuAccess;
        tex->m_usage = usage;

        D3D11_TEXTURE2D_DESC desc;
        memset(&desc,0,sizeof(D3D11_TEXTURE2D_DESC));
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
        if(FAILED( device->CreateTexture2D( &desc, NULL, &pEmptyTex ) ) )
        {
            SAFE_DELETE(tex);
            return 0;
        }

        tex->m_texture = pEmptyTex;

        if(tex->m_flags & D3D11_BIND_RENDER_TARGET)
        {
            D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
			memset(&rtDesc,0,sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
            rtDesc.Format = desc.Format;
            rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtDesc.Texture2D.MipSlice = 0;

            device->CreateRenderTargetView( tex->m_texture, &rtDesc, &tex->m_rtView );
        } else if(tex->m_flags & D3D11_BIND_DEPTH_STENCIL)
        {
			D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
			memset(&dsDesc,0,sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			dsDesc.Format = desc.Format;
			dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsDesc.Texture2D.MipSlice = 0;
			device->CreateDepthStencilView(tex->m_texture, &dsDesc, &tex->m_dsView );

        }

		if (tex->m_flags & D3D11_BIND_UNORDERED_ACCESS){
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			memset(&uavDesc, 0, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
			uavDesc.Format = desc.Format;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = 0;
			device->CreateUnorderedAccessView(tex->m_texture, &uavDesc, &tex->m_uav);
		}

		
        if(tex->m_flags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			memset(&srvDesc,0,sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = desc.MipLevels -1;
            

            device->CreateShaderResourceView( tex->m_texture, &srvDesc, &tex->m_shaderView );
        }
		

        return tex;
        
    }
    Texture* Texture::CreateTextureFromFile(ID3D11Device* device,const string& filepath, ResourceID texname,
        UINT cpuAccess, D3D11_USAGE usage, UINT filter)
    {
        Texture* tex = new Texture(texname);
        D3DX11_IMAGE_LOAD_INFO info;
        info.CpuAccessFlags = cpuAccess;
        info.Usage = usage;
        info.Filter = filter ;
        tex->m_cpuAccess = cpuAccess;
        tex->m_usage = usage;


		std::unique_ptr<WCHAR> filePathWide( multibyteStringToWide(filepath) );


        if(usage!=D3D11_USAGE_STAGING)
        {

			if ( FAILED( D3DX11CreateShaderResourceViewFromFile(  device, filePathWide.get(), &info, NULL, &tex->m_shaderView, NULL ) ) )
            {
				std::string err = std::string("Failed to create resource: ") + filepath;
				showErrorDialog(err.c_str());
                return 0;
            }
            tex->m_shaderView->GetResource(&tex->m_texture);
        }else
        {
            info.BindFlags = 0;
            if(FAILED(D3DX11CreateTextureFromFile(device, filePathWide.get(), &info, 0,&tex->m_texture, 0)))
            {
                std::string err = std::string("Failed to create resource from file: ") + filepath;
				showErrorDialog(err.c_str());
                return 0;
            }
        }
        
        

        D3D11_RESOURCE_DIMENSION type;
        tex->m_texture->GetType(&type);
        switch( type )
        {
            
            case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
            {
                D3D11_TEXTURE2D_DESC desc;
                ID3D11Texture2D *pTexture2D = (ID3D11Texture2D*)tex->m_texture;
                pTexture2D->GetDesc( &desc );
                tex->m_texWidth = desc.Width;
                tex->m_texHeight = desc.Height;
                tex->m_texArraySize = desc.ArraySize;
                tex->m_flags = desc.BindFlags;
                tex->m_format = desc.Format;
            }
            break;
            
            default:
            // we do not atm support other types of textures
            showErrorDialog("Tried to create a texture from file containing unsupported texture type");
            SAFE_DELETE(tex);
            break;
        }
        return tex;
    }

	ResourceID Texture::getName() const
	{
		return m_name;
	}

	ID3D11ShaderResourceView* Texture::GetShaderResourceView() 
	{ 
		return m_shaderView; 
	}

	ID3D11RenderTargetView* Texture::GetRenderTargetView() 
	{ 
		return m_rtView; 
	}

	ID3D11DepthStencilView* Texture::GetDepthStencilView()
	{
		return m_dsView;
	}
	ID3D11UnorderedAccessView*  Texture::GetUnorderedAccessView()
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