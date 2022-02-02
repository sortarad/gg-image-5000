#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// #define LOG_LEVEL   DEBUG
#include "seethe.h"

#include "gamegear_image_converter.h"
#include "rgb.h"
#include "image.h"

int gamegear_save_gg_image_to_file(const char* gg_filename, gamegear_gg_image* gg_image)
{
    int result = 0;
    size_t numwritten;

    if (gg_image == NULL)
    {
        error("gg_image parameter == NULL");
        return -EINVAL;
    }

    FILE* fp = fopen(gg_filename,"wb");
    if (fp == NULL)
    {
        error("Cannot open file for write: %s",gg_filename);
        result = -EIO;
        goto exit_load;
    }

    numwritten = fwrite(gg_image->buffer,1,gg_image->buffer_size,fp);
    if (numwritten != gg_image->buffer_size)
    {
        error("Number of written bytes do not match: (written %zu vs buffer size %zu)",numwritten,gg_image->buffer_size);
        result = -EIO;
        goto exit_load;
    }

exit_load:
    if (fp != NULL)
    {
        fclose(fp);
    }

    return result;
}

gamegear_gg_image* gamegear_load_gg_image_from_file(const char* gg_filename)
{
    bool error = false;

    gamegear_gg_image* gg_image = NULL;
    FILE* fp = fopen(gg_filename,"rb");
    size_t numread;
    long size;

    if (fp == NULL)
    {
        error = true;
        goto exit_load;
    }

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    if (size < 32768)
    {
        error = true;
        goto exit_load;
    }

    gg_image = (gamegear_gg_image*)malloc(sizeof(gamegear_gg_image));
    if (gg_image == NULL)
    {
        error = true;
        goto exit_load;
    }
    gg_image->buffer = (uint8_t*)malloc(size);
    if (gg_image->buffer == NULL)
    {
        error = true;
        goto exit_load;
    }
    gg_image->buffer_size = (size_t)size;

    numread = fread(gg_image->buffer,1,size,fp);
    if (numread != (size_t)size)
    {
        error = true;
        goto exit_load;
    }

exit_load:
    if (fp != NULL)
    {
        fclose(fp);
    }

    if (error)
    {
        if (gg_image != NULL)
        {
            if (gg_image->buffer != NULL)
            {
                free(gg_image->buffer);
            }
            free(gg_image);
            gg_image = NULL;
        }
    }

    return gg_image;
}

/**
 * @brief Convert a RGB palette to a gamegear compatible color palette
 *
 * @param rgb pointer to rgb paletta
 * @param gg pointer to gamegear palette
 * @param gg_palette_size number of colors
 * @return int 0 for success
 *             _EINVAL for invalid arguments
 */
int gamegear_convert_rgb_palette_to_gamegear_palette(
    rgb_palette* rgb,
    gamegear_palette_color* gg,
    size_t gg_palette_size)
{
    size_t i;

    if (rgb == NULL)
    {
        error("rgb == NULL");
        return -EINVAL;
    }

    if (gg == NULL)
    {
        error("gg == NULL");
        return -EINVAL;
    }

    debug("Gamegear palette size : %zu",gg_palette_size);
    for(i=0; i<gg_palette_size; i++)
    {
        rgb_color rgb_clr = rgb->colors[i];
        gamegear_palette_color gg_color;

        gg_color.b = ((rgb_clr.b & 0xF0) >> 4);
        gg_color.r = ((rgb_clr.r & 0xF0) >> 4);
        gg_color.g = ((rgb_clr.g & 0xF0) >> 4);
        gg_color.unused = 0;

        debug("Palette convert: %02X:%02X:%02X => %02X:%02X:%02X",
            rgb_clr.r, rgb_clr.g, rgb_clr.b,
            gg_color.r, gg_color.g, gg_color.b);
        gg[i] = gg_color;
    }

    return 0;
}


int gamegear_image_converter_from_file(
        const char* img_filename,
        const char* gg_filename_in,
        const char* gg_filename_out)
{
    int result = 0;
    size_t w,h;
    int coloridx;
    int temp;

    rgb_image* image = NULL;
    rgb_palette* palette = NULL;
    rgb_color temp_color;
    uint8_t temp_gg_pixel;
    uint8_t* gg_image;
    gamegear_palette_color gamegear_palette[IMAGE_COLOR_PALETTE_SIZE];
    uint16_t x,y;
    uint8_t pattern_temp[32];

    info("gamegear image converter start");

    gamegear_gg_image* gg_file_image = gamegear_load_gg_image_from_file(gg_filename_in);
    if (gg_file_image == NULL)
    {
        error("Cannot load gamegear gg file: %s",gg_filename_in);
        result = -ENOMEM;
        goto exit_converter;
    }
    info("gamegear gg file loaded: %s",gg_filename_in);

    image = rgb_loadfile(img_filename,160,144);
    if (image == NULL)
    {
        error("Cannot load rgb file: %s",img_filename);
        result = -ENOMEM;
        goto exit_converter;
    }
    info("RGB image file loaded: %s",img_filename);

    palette = rgb_get_palette(image);
    if (palette == NULL)
    {
        error("Failure in extracting RGB palette");
        result = -ENOMEM;
        goto exit_converter;
    }
    info("RGB palette constructed (%d colors)",palette->num_colors);

    gg_image = (uint8_t*)malloc(IMAGE_BYTE_SIZE);
    if (gg_image == NULL)
    {
        error("Failure in allocating gamegear bitmap image");
        result = -ENOMEM;
        goto exit_converter;
    }
    info("Gamegear bitmap image allocated");

    debug("Converting image ....");
    memset(gg_image,0,IMAGE_BYTE_SIZE);
    for(h=0; h<image->height; h++)
    {
        for(w=0; w<image->width; w++)
        {
            temp_gg_pixel = gg_image[(w/2) + (IMAGE_BYTE_WIDTH*h)];

            rgb_pixel p = image->pixels[w + h*image->width];
            rgb_pixel_2_color(&temp_color, &p);
            coloridx = rgb_find_color_in_palette(palette,&temp_color);
            if (coloridx >= 0)
            {
                coloridx = coloridx & 0x0F;
                if (w & 0x01)
                {
                    temp_gg_pixel |= (uint8_t)coloridx;
                }
                else
                {
                    temp_gg_pixel |= (((uint8_t)coloridx) << 4);
                }
                gg_image[(w/2) + (IMAGE_BYTE_WIDTH*h)] = temp_gg_pixel;
            }
        }
    }
    info("bitmap conversion completed");

    debug("Converting RGB palette to gamegear compatible color palette");
    temp = gamegear_convert_rgb_palette_to_gamegear_palette(
        palette,
        gamegear_palette,
        IMAGE_COLOR_PALETTE_SIZE);
    if (temp != 0)
    {
        debug("Palette conversion failed (%d)",temp);
        result = temp;
        goto exit_converter;
    }
    info("RGB palette to gamegear compatible color palette completed");

    // copy in the image
    debug("Copy bitmap image");
    //memcpy(gg_file_image->buffer + IMAGE_BITMAP_ROM_ADDRESS, gg_image, IMAGE_BYTE_SIZE);
    for(x=0; x<IMAGE_CHAR_WIDTH; x++)
    {
        for(y=0; y<IMAGE_CHAR_HEIGHT; y++)
        {
            image_get_pattern_tile(gg_image,(uint8_t)x, (uint8_t)y,pattern_temp);
            memcpy(
                gg_file_image->buffer + IMAGE_BITMAP_ROM_ADDRESS + (x*32) + (y * 640),
                pattern_temp, sizeof(pattern_temp));
            //vdp_write_vram_buffer(x*32 + (y * (640)), );
            //vdp_write_vram_buffer(0x420, pattern_temp, sizeof(pattern_temp));
        }
    }
    // copy in the palette
    debug("Copy gamegear palette");
    memcpy(gg_file_image->buffer + IMAGE_PALETTE_ROM_ADDRESS, gamegear_palette, sizeof(gamegear_palette));

    // write the result to file
    if (gamegear_save_gg_image_to_file(gg_filename_out, gg_file_image) != 0)
    {
        result = -EIO;
        goto exit_converter;
    }

    result = 0;

exit_converter:
    if (gg_file_image != NULL)
    {

    }

    if (image != NULL)
    {
        free(image);
    }

    if (palette != NULL)
    {
        free(palette);
    }
    return result;
}

