#include <xc.h>
#include "i2c_master.h"

// addresses and registers
#define DEV_ADDR        0x57 // @todo we probably not use, delete it?
#define DEV_WRITE       0xAE
#define DEV_READ        0xAF
#define REG_INT_STATUS1 0x00
#define REG_INT_STATUS2 0x01
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA   0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED2_PA     0x0D     // IR current
#define REG_PART_ID     0xFF

// configs
#define CFG_SPO2  0b0100111  // 4096 nA, 100 sps, 18 bits 
#define CFG_FIFO  0b01001111 // avg=4, no roll-over, AFULL=15
#define CFG_MODE  0b010      // HR mode, red only
#define CFG_PA    0x24       // 7.2 mA?
#define CFG_RESET 0x40       // reset max30102

/**
 * @brief Write 1 byte data to reg
 * @param reg
 * @param data
 */
void write8(uint8_t reg, uint8_t data) {
    I2C_start();
    I2C_write(DEV_WRITE);
    I2C_write(reg);
    I2C_write(data);
    I2C_stop();
}

/**
 * @brief Read 1 byte data from reg
 * @param reg
 * @return read byte
 */
uint8_t read8(uint8_t reg) {
    uint8_t v;
    I2C_start();
    I2C_write(DEV_WRITE);
    I2C_write(reg);
    I2C_rep_start();
    I2C_write(DEV_READ);
    v = I2C_read(1); // NACK
    I2C_stop();
    return v;
}
