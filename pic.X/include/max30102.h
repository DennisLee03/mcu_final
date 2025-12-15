#ifndef MAX30102_H
#define MAX30102_H

#include <stdint.h>
#include <stdbool.h>

#define MAX30102_ADDR_WRITE 0xAE
#define MAX30102_ADDR_READ  0xAF

// Register Addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR   0x04
#define REG_OVF_COUNTER   0x05
#define REG_FIFO_RD_PTR   0x06
#define REG_FIFO_DATA     0x07
#define REG_FIFO_CONFIG   0x08
#define REG_MODE_CONFIG   0x09
#define REG_SPO2_CONFIG   0x0A
#define REG_LED1_PA       0x0C
#define REG_LED2_PA       0x0D
#define REG_PART_ID       0xFF

bool max30102_initialize(void);
void max30102_write_reg(uint8_t reg, uint8_t value);
uint8_t max30102_read_reg(uint8_t reg);
void max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);
uint8_t max30102_available(void); // Returns number of samples available

#endif