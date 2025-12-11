#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

void I2C_wait_idle(void);
void I2C_start(void);
void I2C_rep_start(void);
void I2C_stop(void);
void I2C_init(void);
void I2C_write(uint8_t data);
uint8_t I2C_read(uint8_t ack);

#endif	/* I2C_MASTER_H */

