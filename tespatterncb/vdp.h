#ifndef VDP_H
#define VDP_H

#include <stdint.h>
#include "cassert.h"

#define VDP_CONTROL_PORT_ADDR 0xBF
#define VDP_DATA_PORT_ADDR    0xBE

#define VDP_STATUS_REG_IFLAG      (0x80)
#define VDP_STATUS_REG_9TH_SPRITE (0x40)
#define VDP_STATUS_REG_COLLISION  (0x20)

#define VDP_RGB(r,g,b) ((r) | ((g) << 4) | ((b) << 8))

/*
 *  register definitions
 */
#define VDP_REG0_BASE_VALUE (0x06)
#define VDP_REG0_MVS (0x80)
#define VDP_REG0_IE1 (0x10)
#define VDP_REG0_EC  (0x08)

#define VDP_REG1_BASE_VALUE (0x80)
#define VDP_REG1_BLANK (0x40)
#define VDP_REG1_IE    (0x20)
#define VDP_REG1_SIZE  (0x02)

#define VDP_REG2_BASE_0000H (0xF1)
#define VDP_REG2_BASE_0800H (0xF3)
#define VDP_REG2_BASE_1000H (0xF5)
#define VDP_REG2_BASE_1800H (0xF7)
#define VDP_REG2_BASE_2000H (0xF9)
#define VDP_REG2_BASE_2800H (0xFB)
#define VDP_REG2_BASE_3000H (0xFD)
#define VDP_REG2_BASE_3A00H (0xFF)

#define VDP_REG3_BASE_VALUE (0xFF)
#define VDP_REG4_BASE_VALUE (0xFF)

#define VDP_REG5_SPRITE_BASEADDR(x) (0x81 + ((x) / 0x80))

#define VDP_REG6_BASE_0000H (0xFB)
#define VDP_REG6_BASE_2000H (0xFF)

typedef struct
{
    uint8_t reg;    // register
    uint8_t data;   // data to write to register
} vdp_init_reg_st;
CASSERT(sizeof(vdp_init_reg_st) == 2, sizeof_vdp_init_reg_st_not_correct)

/**
 * @brief read the vdp status register
 *
 * @return uint8_t status register value
 */
uint8_t vdp_read_status_register(void);

/**
 * @brief write vdp register
 *
 * @param reg register number to write to
 * @param data data to write to register
 */
void vdp_write_reg(uint8_t reg, uint8_t data);

/**
 * @brief write data to VDP RAM
 *
 * @param addr VDP address to write to
 * @param data data to write
 */
void vdp_write_vram(uint16_t addr, uint8_t data);

/**
 * @brief write buffer to color ram
 *
 * @param addr color ram address
 * @param rgbdata pointer to rgb table
 * @param len length of table
 */
void vdp_write_color_ram_buffer(uint16_t addr, uint16_t* rgbdata, uint16_t len);

/**
 * @brief write buffer to vdp RAM
 *
 * @param addr VDP address to write to
 * @param data pointer to data to write
 * @param len number of bytes to write
 */
void vdp_write_vram_buffer(uint16_t addr, uint8_t* data, uint16_t len);

/**
 * @brief read a byte from VDP RAM
 *
 * @param addr VDP address to read from
 * @return uint8_t data read at the VDP address
 */
uint8_t vdp_read_vram(uint16_t addr);

/**
 * @brief read data from VDP ram to a buffer
 *
 * @param addr VDP address to read from
 * @param buf dest buffer to write to
 * @param len number of bytes to vdp_read_vram_buffer
 *
 * @note the calling function should ensure that buf points to a big enough buffer
 */
void vdp_read_vram_buffer(uint16_t addr, uint8_t* buf, uint16_t len);

#endif // VDP_H
