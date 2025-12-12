#include <xc.h>
#include "i2c_master.h"

#define DEV_ADDR        0x57 // @todo we probably not use, delete it?
#define DEV_WRITE       0xAE
#define DEV_READ        0xAF
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA   0x07
#define REG_MODE        0x09