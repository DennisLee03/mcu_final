#ifndef I2C_H
#define I2C_H

#include <xc.h>
#include <stdint.h>

void i2c_initialize(void);
void i2c_is_idle(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);

#endif