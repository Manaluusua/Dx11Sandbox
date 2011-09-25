#include "PixelBox.h"

namespace Dx11Sandbox
{
    PixelBox::PixelBox()
        :m_width(0),
        m_height(0),
        m_pixels(0)
    {

    }

    void PixelBox::allocatePixelBox(UINT width,UINT height)
    {
        m_width = width;
        m_height = height;
        
        SAFE_DELETE(m_pixels);

        m_pixels = new Pixel[width*height];
    }


    PixelBox::~PixelBox()
    {
        SAFE_DELETE(m_pixels);
    }

    PixelBox::Pixel& PixelBox::getPixel(int x, int y) const
    {
        return m_pixels[y*m_width + x];
    }
}