#ifndef DX11SANDBOX_PIXELBOX_H
#define DX11SANDBOX_PIXELBOX_H
#include "DXUT.h"
#include "CommonUtilities.h"
namespace Dx11Sandbox
{

    class PixelBox
    {
    public:

        struct Pixel
        {
            UCHAR r;
            UCHAR g;
            UCHAR b;
            UCHAR a;
        };

        PixelBox();
        ~PixelBox();
        void allocatePixelBox(UINT width,UINT height);

        //only getters. Only way to set members on pixelbox is to use allocatePixelBox function
        UINT getHeight() const {return m_height;}
        UINT getWidth() const {return m_width;}
        Pixel* getPixels() {return m_pixels;}

        Pixel& getPixel(int x, int y) const;

    private:
        DISABLE_COPY(PixelBox)
        UINT m_height;
        UINT m_width;
        Pixel *m_pixels;
        
    };
}
#endif