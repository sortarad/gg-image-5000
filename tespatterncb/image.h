#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <stdint.h>

#ifdef BUILD_TYPE_SCREEN_TEST
#define IMAGE_PIXEL_WIDTH  (160)
#define IMAGE_PIXEL_HEIGHT (80)
#define IMAGE_BYTE_WIDTH  (IMAGE_PIXEL_WIDTH / 2)
#define IMAGE_CHAR_WIDTH  (IMAGE_PIXEL_WIDTH / 8)
#define IMAGE_CHAR_HEIGHT (IMAGE_PIXEL_HEIGHT / 8)

#define IMAGE_BYTE_SIZE (IMAGE_PIXEL_HEIGHT * IMAGE_BYTE_WIDTH)

#else
#define IMAGE_PIXEL_WIDTH  (160)
#define IMAGE_PIXEL_HEIGHT (144)
#define IMAGE_BYTE_WIDTH  (IMAGE_PIXEL_WIDTH / 2)
#define IMAGE_CHAR_WIDTH  (IMAGE_PIXEL_WIDTH / 8)
#define IMAGE_CHAR_HEIGHT (IMAGE_PIXEL_HEIGHT / 8)

#define IMAGE_BYTE_SIZE (IMAGE_PIXEL_HEIGHT * IMAGE_BYTE_WIDTH)

#endif // BUILD_TYPE_SCREEN_TEST

#define IMAGE_COLOR_PALETTE_SIZE (16)
#define IMAGE_BITMAP_ROM_ADDRESS (0x1100)
#define IMAGE_PALETTE_ROM_ADDRESS (0x1000)

/**
 * @brief put pixel on image
 *
 * @param img pointer to image buffer
 * @param x x position of pixel
 * @param y y position of pixel
 * @param color color value (0-16)
 */
void image_put_pixel(uint8_t* img, uint8_t x, uint8_t y, uint8_t color);

/**
 * @brief get pixel from image
 *
 * @param img pointer to image buffer
 * @param x x position of pixel
 * @param y y position of pixel
 *
 * @return pixel color value
 */
uint8_t image_get_pixel(uint8_t* img, uint8_t x, uint8_t y);

/**
 * @brief put pixel on image
 *
 * @param img pointer to image buffer
 * @param x x position of pixel
 * @param y y position of pixel
 * @param pattern pointer to tile buffer (must point to a buffer of 32 bytes)
 */
void image_get_pattern_tile(uint8_t* img, uint8_t x, uint8_t y, uint8_t* pattern);

#endif // IMAGE_H_INCLUDED
