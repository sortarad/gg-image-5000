#ifndef GAMEGEAR_H
#define GAMEGEAR_H

#include <stdint.h>
#include "cassert.h"

typedef enum
{
    GAMEGEAR_DEVICE_SMS_ID = 0x4c,
    GAMEGEAR_DEVICE_GG_ID = 0x6c
} gamegear_device_id_en;
CASSERT(sizeof(gamegear_device_id_en) == 1, sizeof_gamegear_device_id_en_not_correct)

typedef struct
{
    char     idstr[9]; // the 9th char MUST be zero and is actually part of a reserved value
    uint8_t  reserved;
    uint16_t crc;
    uint8_t  reserved2[3];
    uint8_t  deviceid;

} gamegear_id_block_st;
CASSERT(sizeof(gamegear_id_block_st) == 16, sizeof_gamegear_id_block_st_en_not_correct)

extern __at 0x7FF0 const gamegear_id_block_st gamegear_id_block;

#endif // GAMEGEAR_H
