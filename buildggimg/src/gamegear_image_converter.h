#ifndef GAMEGEAR_IMAGE_CONVERTER_H
#define GAMEGEAR_IMAGE_CONVERTER_H

#include "rgb.h"

typedef struct
{
    uint8_t* buffer;
    size_t buffer_size;
} gamegear_gg_image;

typedef struct
{
    uint16_t r : 4;
    uint16_t g : 4;
    uint16_t b : 4;
    uint16_t unused : 4;
} gamegear_palette_color;

/**
 * @brief convert a rgb_image object to game gear image
 *
 * @param img
 * @return int 0 for success, otherwise error
 */
int gamegear_image_converter_from_rgb(rgb_image* img);

/**
 * @brief convert a RBG image and patch into a gg file
 * 
 * @param img_filename char pointer to filename of image
 * @param gg_filename_in char pointer to input gg filename
 * @param gg_filename_out char pointer to output gg filename
 * @return int 0 for success
 *             other for error
 */
int gamegear_image_converter_from_file(
        const char* img_filename,
        const char* gg_filename_in,
        const char* gg_filename_out);

#endif // GAMEGEAR_IMAGE_CONVERTER_H