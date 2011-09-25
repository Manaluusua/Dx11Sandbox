#include "Texture.h"
#include "SDKmisc.h"
#include "PixelBox.h"

namespace Dx11Sandbox
{
    Texture::Texture(const wstring texname)
        :m_name(texname),
        m_shaderView(0),
        m_rtView(0),
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
    }

    Texture* Texture::CreateEmptyTexture2D(ID3D11Device* device, const wstring& texname, UINT texWidth, UINT texHeight,
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
        desc.Width = texWidth;
        desc.Height = texHeight;
        desc.MipLevels = 1;
        desc.ArraySize = arraySize;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.CPUAccessFlags = cpuAccess;
        desc.Usage = usage;
        desc.BindFlags = bindFlags;

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
            rtDesc.Format = desc.Format;
            rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtDesc.Texture2D.MipSlice = 0;

            device->CreateRenderTargetView( tex->m_texture, &rtDesc, &tex->m_rtView );
        }
        if(tex->m_flags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = desc.MipLevels -1;
            

            device->CreateShaderResourceView( tex->m_texture, &srvDesc, &tex->m_shaderView );
        }
        if(tex->m_flags & D3D11_BIND_DEPTH_STENCIL)
        {
            //create depth stencil view
        }

        return tex;
        
    }
    Texture* Texture::CreateTextureFromFile(ID3D11Device* device,const wstring& filename, const wstring& texname,
        UINT cpuAccess, D3D11_USAGE usage)
    {
        Texture* tex = new Texture(texname);
        D3DX11_IMAGE_LOAD_INFO info;
        info.CpuAccessFlags = cpuAccess;
        info.Usage = usage;
        info.Filter = D3DX11_FILTER_POINT | D3DX11_FILTER_SRGB_IN ;
        tex->m_cpuAccess = cpuAccess;
        tex->m_usage = usage;
        WCHAR wstr[MAX_PATH];

        if FAILED( DXUTFindDXSDKMediaFileCch( wstr, MAX_PATH, filename.c_str()))
        {
            showErrorDialog("Tex not found!");
            return 0;
        }

        if(usage!=D3D11_USAGE_STAGING)
        {

            if ( FAILED( D3DX11CreateShaderResourceViewFromFile(  device, wstr, &info, NULL, &tex->m_shaderView, NULL ) ) )
            {
                showErrorDialog("Failed to create resource view!");
                return 0;
            }
            tex->m_shaderView->GetResource(&tex->m_texture);
        }else
        {
            info.BindFlags = 0;
            if(FAILED(D3DX11CreateTextureFromFile(device, wstr, &info, 0,&tex->m_texture, 0)))
            {
                showErrorDialog("Failed to create resource texture!");
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