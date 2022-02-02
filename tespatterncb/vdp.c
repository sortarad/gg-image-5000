#include <stdint.h>
#include "vdp.h"
#include "z80.h"

__SFRDEF(VDP_CONTROL_PORT,VDP_CONTROL_PORT_ADDR)
__SFRDEF(VDP_DATA_PORT,VDP_DATA_PORT_ADDR)

uint8_t vdp_read_status_register(void)
{
    uint8_t result = VDP_CONTROL_PORT;

    return result;
}

void vdp_write_reg(uint8_t reg, uint8_t data)
{
    (void)vdp_read_status_register();
    VDP_CONTROL_PORT = data;
    VDP_CONTROL_PORT = (0x80 | (reg & 0x0F));
}

void vdp_write_vram(uint16_t addr, uint8_t data)
{
    VDP_CONTROL_PORT = (addr & 0xFF);
    VDP_CONTROL_PORT = (((addr & 0x3F00) >> 8) | 0x40);

    // delay
    __asm
    push ix
    pop ix
    __endasm;

    VDP_DATA_PORT = data;

    __asm
    push af
    pop af
    __endasm;
}

void vdp_write_color_ram_buffer(uint16_t addr, uint16_t* rgbdata, uint16_t len)
{
    uint16_t i;

    VDP_CONTROL_PORT = (addr & 0xFF);
    VDP_CONTROL_PORT = 0xC0;

    // delay
    __asm
    push af
    pop af
    __endasm;

    for(i=0; i<len; i++)
    {
        uint8_t gr = ((*rgbdata) & 0xFF);
        uint8_t b = (((*rgbdata) >> 8) & 0xFF);

        VDP_DATA_PORT = gr;

        // delay
        __asm
        push af
        pop af
        __endasm;

        VDP_DATA_PORT = b;


        __asm
        push af
        pop af
        __endasm;

        rgbdata++;
    }

    __asm
    push ix
    pop ix
    __endasm;
}

void vdp_write_vram_buffer(uint16_t addr, uint8_t* data, uint16_t len)
{
    uint16_t i;
    // volatile uint8_t temp = 7;

    VDP_CONTROL_PORT = (addr & 0xFF);
    VDP_CONTROL_PORT = (((addr & 0x3F00) >> 8) | 0x40);

    for(i=0; i<len; i++)
    {
        // delay
        __asm
        push ix
        pop ix
        push ix
        pop ix
        __endasm;

        VDP_DATA_PORT = *data;

        // delay
        __asm
        push ix
        pop ix
        push ix
        pop ix
        __endasm;

        // temp = VDP_DATA_PORT;

        __asm
        push af
        pop af
        push af
        pop af
        __endasm;

        data++;
    }
    // delay
    __asm
    push ix
    pop ix
    __endasm;
}

uint8_t vdp_read_vram(uint16_t addr)
{
    uint8_t result;

    VDP_CONTROL_PORT = (addr & 0xFF);
    VDP_CONTROL_PORT = ((addr & 0x3F00) >> 8);

    // delay
    __asm
    push ix
    pop ix
    __endasm;

    result = VDP_DATA_PORT;

    // delay
    __asm
    push ix
    pop ix
    __endasm;

    return result;
}

void vdp_read_vram_buffer(uint16_t addr, uint8_t* buf, uint16_t len)
{
    uint16_t i;

    VDP_CONTROL_PORT = (addr & 0xFF);
    VDP_CONTROL_PORT = ((addr & 0x3F00) >> 8);

    __asm
    push ix
    pop ix
    __endasm;

    for(i=0; i<len; i++)
    {
        *buf = VDP_DATA_PORT;

        // delay
        __asm
        push ix
        pop ix
        __endasm;

        buf++;
    }
}


