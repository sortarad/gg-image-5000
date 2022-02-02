#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rgb.h"

rgb_image* rgb_loadfile(const char* filename, size_t width, size_t height)
{
    FILE *fp = fopen(filename, "r");
    rgb_image* result = NULL;

    if (fp == NULL)
    {
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    result = (rgb_image*)malloc(sizeof(rgb_image));
    result->pixels = (rgb_pixel*)malloc((size_t)size);

    fread(result->pixels,1,size,fp);

    result->height = height;
    result->width = width;

    fclose(fp);

    return result;
}

/**
 * @brief free RGB memory
 *
 */
void rgb_free(rgb_image* rgbfile)
{
    if (rgbfile == NULL)
    {
        return;
    }

    if (rgbfile->pixels != NULL)
    {
        free(rgbfile->pixels);
    }

    free(rgbfile);
}

int rgb_find_color_in_palette(rgb_palette* palette, rgb_color* color)
{
    int i;

    if (palette == NULL)
    {
        return -EINVAL;
    }

    if (color == NULL)
    {
        return -EINVAL;
    }

    for(i=0; i<palette->num_colors; i++)
    {
        if (memcmp(&(palette->colors[i]), color, sizeof(rgb_color)) == 0)
        {
            return i;
        }
    }

    return -ENOENT;
}

int rgb_add_color_to_palette(rgb_palette* palette, rgb_color* color)
{
    if (palette == NULL)
    {
        return -EINVAL;
    }

    if (color == NULL)
    {
        return -EINVAL;
    }

    if (palette->num_colors >= RGB_PALETTE_MAX_COLORS)
    {
        palette->num_colors = RGB_PALETTE_MAX_COLORS;
        return -ENOMEM;
    }

    memcpy(&(palette->colors[palette->num_colors]), color, sizeof(rgb_color));
    palette->num_colors++;

    return 0;
}

void rgb_color_2_pixel(rgb_pixel* pixel, rgb_color* color)
{
    pixel->b = color->b;
    pixel->r = color->r;
    pixel->g = color->g;
}

void rgb_pixel_2_color(rgb_color* color, rgb_pixel* pixel)
{
    color->b = pixel->b;
    color->r = pixel->r;
    color->g = pixel->g;
}

rgb_palette* rgb_get_palette(rgb_image* img)
{
    size_t i;
    rgb_color temp_color;

    if (img == NULL)
    {
        return NULL;
    }

    rgb_palette* palette = (rgb_palette*)malloc(sizeof(rgb_palette));

    // count all the colours
    palette->num_colors = 0;
    memset(palette->colors,0,sizeof(palette->colors));

    for(i=0; i<(img->height * img->width); i++)
    {
        rgb_pixel_2_color(&temp_color, &(img->pixels[i]));
        if (rgb_find_color_in_palette(palette, &temp_color) < 0)
        {
            // color is not in palette
            rgb_add_color_to_palette(palette, &temp_color);
        }
    }

    return palette;
}
