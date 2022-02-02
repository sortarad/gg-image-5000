#ifndef RGB_H
#define RGB_H

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "cassert.h"

#define RGB_PALETTE_MAX_COLORS (256)

#ifdef _MSC_VER
#pragma pack(push,1)
typedef struct
#else
typedef struct __attribute__((packed))
#endif
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_pixel;
CASSERT(sizeof(rgb_pixel)==3,sizeof_rgb_pixel_not_correct)
#ifdef _MSC_VER
#pragma pack(pop)
#endif

typedef struct
{
    rgb_pixel* pixels;
    size_t width;
    size_t height;
} rgb_image;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_color;

typedef struct
{
    rgb_color colors[RGB_PALETTE_MAX_COLORS];
    int num_colors;
} rgb_palette;

/**
 * @brief load a rgb file and allocate memory for it
 *
 * @param filename
 * @return rgb_file* pointer to rgb file struct (NULL if not allocated or error)
 */
rgb_image* rgb_loadfile(const char* filename, size_t width, size_t height);

/**
 * @brief free RGB memory
 *
 * @param img to free
 *
 */
void rgb_free(rgb_image* img);

/**
 * @brief derive the palette from the image
 *
 * @param img
 * @return rgb_palette* rgb palette object (NULL if not allocated)
 */
rgb_palette* rgb_get_palette(rgb_image* img);

/**
 * @brief find a color in the palette
 *
 * @param palette pointer to the palette
 * @param color pointer to the color
 * @return int index in the palette,
 *             -ENOENT - not found
 *             -EINVAL - invalid arguments
 */
int rgb_find_color_in_palette(rgb_palette* palette, rgb_color* color);

/**
 * @brief add a color to the palette
 *
 * @param palette
 * @param color
 * @return int 0 = success
 *         -EINVAL - invalid arguments
 *         -ENOMEM - no more space for a color
 */
int rgb_add_color_to_palette(rgb_palette* palette, rgb_color* color);

/**
 * @brief convert a color to a pixel
 *
 * @param pixel pointer to the result pixel
 * @param color pointer to the color to convert
 */
void rgb_color_2_pixel(rgb_pixel* pixel, rgb_color* color);

/**
 * @brief convert a pixel to a color
 *
 * @param color pointer to the result color
 * @param pixel pointer to the pixel to convert
 */
void rgb_pixel_2_color(rgb_color* color, rgb_pixel* pixel);

#endif // RGB_H
