#include <stdint.h>
#include "image.h"

void image_put_pixel(uint8_t* img, uint8_t x, uint8_t y, uint8_t color)
{
    uint8_t x0 = x / 2;
    uint16_t offs = ((uint16_t)y * IMAGE_BYTE_WIDTH) + x0;

    uint8_t pixel = img[offs];

    if (x & 0x01)
    {
        pixel = (pixel & 0xF0) | (color & 0x0F);
    }
    else
    {
        pixel = (pixel & 0x0F) | ((color & 0x0F) << 4);
    }
    img[offs] = pixel;

}

uint8_t image_get_pixel(uint8_t* img, uint8_t x, uint8_t y)
{
    uint8_t x0 = x / 2;
    uint16_t offs = ((uint16_t)y * IMAGE_BYTE_WIDTH) + x0;

    uint8_t pixel = img[offs];

    if (x & 0x01)
    {
        pixel = (pixel & 0x0F);
    }
    else
    {
        pixel = ((pixel & 0xF0) >> 4);
    }

    return pixel;
}

// x is char
// y is char
void image_get_pattern_tile(uint8_t* img, uint8_t x, uint8_t y, uint8_t* pattern)
{
    uint8_t x0 = x * 8;
    uint8_t y0 = y * 8;

    uint8_t ix, iy, p;
    uint8_t pixel;

    uint8_t bpp0, bpp1, bpp2, bpp3;

    for(p=0; p < 32; p++)
    {
        pattern[p] = 0;
    }

    for(ix = 0; ix<8; ix++)
    {
        for(iy = 0; iy<8; iy++)
        {
            pixel = image_get_pixel(img,x0 + ix,y0+iy);
            bpp0 = pixel & 0x01;
            bpp1 = ((pixel & 0x02) >> 1);
            bpp2 = ((pixel & 0x04) >> 2);
            bpp3 = ((pixel & 0x08) >> 3);

            bpp0 <<= (7 - ix);
            bpp1 <<= (7 - ix);
            bpp2 <<= (7 - ix);
            bpp3 <<= (7 - ix);

            pattern[(iy * 4)+0] |= bpp0;
            pattern[(iy * 4)+1] |= bpp1;
            pattern[(iy * 4)+2] |= bpp2;
            pattern[(iy * 4)+3] |= bpp3;
        }
    }

}
