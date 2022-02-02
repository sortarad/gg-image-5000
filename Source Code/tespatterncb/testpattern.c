#include "vdp.h"
#include "util.h"
#include "cassert.h"
#include "image.h"

// define BUILD_TYPE_SCREEN_TEST in buildt to compile for test app to show test image

#ifdef BUILD_TYPE_SCREEN_TEST
uint8_t img[IMAGE_BYTE_SIZE];
uint8_t pattern_temp[32];
#else
const __at 0x1100 uint8_t display_image[IMAGE_BYTE_SIZE];
#endif // BUILD_TYPE_SCREEN_TEST

vdp_init_reg_st vdp_setup_table[] =
{
    { 0, VDP_REG0_BASE_VALUE },
    { 1, VDP_REG1_BASE_VALUE | VDP_REG1_BLANK | VDP_REG1_IE },
    { 2, 0xFF },
    { 3, 0xFF },
    { 4, 0xFF },
    { 5, 0xFF },
    { 6, 0x00 },
    { 7, 0x00 },
    { 8, 0x00 },
    { 9, 0x00 },
    { 10, 0xFF }
};
CASSERT(util_sizeof_array(vdp_setup_table) == 11, util_sizeof_array_vdp_setup_table_not_correct)

#ifdef BUILD_TYPE_SCREEN_TEST
uint16_t color_ram_table[] =
{
    // BGR
    0x0000,
    0x000A,
    0x00A0,
    0x00AA,
    0x0A00,
    0x0A0A,
    0x0AA0,
    0x0AAA,
    0x0888,
    0x000F,
    0x00F0,
    0x00FF,
    0x0F00,
    0x0F0F,
    0x0FF0,
    0x0FFF,
};
CASSERT(util_sizeof_array(color_ram_table) == 16, util_sizeof_array_vdp_setup_table_not_correct)
#else
const __at 0x1000 uint16_t color_ram_table[16];
#endif // BUILD_TYPE_SCREEN_TEST

void delay(int x)
{
    volatile int i;

    for(i=0; i<x; i++);
}

void init_vdp(void)
{
    uint8_t i;

    // write setup registers
    for(i=0; i<util_sizeof_array(vdp_setup_table); i++)
    {
        vdp_write_reg(vdp_setup_table[i].reg,vdp_setup_table[i].data);
    }

    vdp_write_color_ram_buffer(0,color_ram_table,util_sizeof_array(color_ram_table));
}

#ifdef BUILD_TYPE_SCREEN_TEST
void init_pattern_generator_table_test(void)
{
    uint16_t x,y;

    for(x=0; x<IMAGE_CHAR_WIDTH; x++)
    {
        for(y=0; y<IMAGE_CHAR_HEIGHT; y++)
        {
            image_get_pattern_tile(img,x,y,pattern_temp);

            vdp_write_vram_buffer(x*32 + (y * (640)), pattern_temp, sizeof(pattern_temp));
            //vdp_write_vram_buffer(0x420, pattern_temp, sizeof(pattern_temp));
        }
    }
}

void init_test_screen1(void)
{
    uint8_t x,y, ix, iy;
    uint8_t color = 0;

    for(y = 0; y<IMAGE_PIXEL_HEIGHT; y+=8)
    {
        for(x = 0; x<IMAGE_PIXEL_WIDTH; x+=16)
        {
            for(iy = 0; iy<8; iy++)
            {
                color = iy;
                for(ix = 0; ix<16; ix++)
                {
                    image_put_pixel(img,x+ix,y+iy,color);
                    color++;
                    if (color >= 16)
                    {
                        color = 0;
                    }
                }
            }
        }
    }
}

void init_test_screen2(void)
{
    uint8_t x,y, ix, iy;
    uint8_t color = 0;

    for(y = 0; y<IMAGE_PIXEL_HEIGHT; y+=4)
    {
        color = (y & 0x0F);
        for(x = 0; x<IMAGE_PIXEL_WIDTH; x+=4)
        {
            for(iy = 0; iy<4; iy++)
            {
                for(ix = 0; ix<4; ix++)
                {
                    image_put_pixel(img,x+ix,y+iy,color);
                }
            }
            color++;
            if (color >= 16)
            {
                color = 0;
            }
        }
    }
}

void init_test_screen(uint8_t idx)
{
    if (idx == 1)
    {
        init_test_screen1();
    }
    else if (idx == 2)
    {
        init_test_screen2();
    }
}

void test_vdp(void)
{
    init_test_screen(2);
    init_pattern_generator_table_test();
    init_fill_pattern_name_table();
}

void init_fill_pattern_name_table(void)
{
    uint16_t i;
    uint8_t val = 0;

    for(i=0x3800; i<(0x3800 + 1792); i+=2)
    {
        vdp_write_vram(i,val);
        vdp_write_vram(i+1,0);
        val++;

        if (val > 0xC7)
        {
            val = 0;
        }
    }
}

#endif // BUILD_TYPE_SCREEN_TEST

#ifndef BUILD_TYPE_SCREEN_TEST
void init_fill_pattern_name_table(void)
{
    uint16_t x,y,i;
    uint16_t val = 0;

    for(y=0; y<18; y++)
    {
        for(x=0; x<20; x++)
        {
            i = y*64 + x*2 + 0x38CC;
            vdp_write_vram(i,val & 0xFF);
            vdp_write_vram(i+1,(val & 0xFF00) >> 8);

            val++;
            if (val > (20*18))
            {
                val = 0;
            }
        }
    }
}

void copy_image_to_vdp(void)
{
    vdp_write_vram_buffer(0x0000, display_image, sizeof(display_image));
    init_fill_pattern_name_table();
}
#endif // BUILD_TYPE_SCREEN_TEST

int main(void)
{
    __asm
    di
    im 1
    __endasm;

    init_vdp();

#ifdef BUILD_TYPE_SCREEN_TEST
    test_vdp();
#else
    copy_image_to_vdp();
#endif // BUILD_TYPE_SCREEN_TEST

    while (1)
    {
        delay(10000);
#ifdef BUILD_TYPE_SCREEN_TEST
        color_ram_table[15] ^= 0x001;

        vdp_write_color_ram_buffer(0,color_ram_table,util_sizeof_array(color_ram_table));
#endif // BUILD_TYPE_SCREEN_TEST
    }

    // return 0;
}
