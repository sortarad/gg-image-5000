
#include "gamegear.h"

__at 0x7FF0 const gamegear_id_block_st gamegear_id_block =
{
    .idstr = "TMR SEGA",
    .reserved = 0,
    .crc = 0,
    .reserved2 = { 0 },
    .deviceid = GAMEGEAR_DEVICE_GG_ID,
};
