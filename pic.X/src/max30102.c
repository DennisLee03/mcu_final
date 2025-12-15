#include "max30102.h"
#include "i2c.h"
#include "uart.h" // For debugging prints if needed
#include "config.h"

void max30102_write_reg(uint8_t reg, uint8_t value) {
    i2c_start();
    i2c_write(MAX30102_ADDR_WRITE);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

uint8_t max30102_read_reg(uint8_t reg) {
    uint8_t val;
    i2c_start();
    i2c_write(MAX30102_ADDR_WRITE);
    i2c_write(reg);
    i2c_rep_start();
    i2c_write(MAX30102_ADDR_READ);
    val = i2c_read(0); // NACK
    i2c_stop();
    return val;
}

bool max30102_initialize(void) {
    // Check Part ID
    uint8_t part_id = max30102_read_reg(REG_PART_ID);
    if (part_id != 0x15) {
        return false;
    }
    /*
    // Reset
    max30102_write_reg(REG_MODE_CONFIG, 0x40);
    // Note: Should wait a bit here in main or use a delay

    // FIFO Config
    max30102_write_reg(REG_FIFO_WR_PTR, 0x00);
    max30102_write_reg(REG_OVF_COUNTER, 0x00);
    max30102_write_reg(REG_FIFO_RD_PTR, 0x00);
    max30102_write_reg(REG_FIFO_CONFIG, 0x10); // Rollover enabled

    // SpO2 Config
    // Range=4096nA, 100SPS, PulseWidth=411us
    max30102_write_reg(REG_SPO2_CONFIG, 0x47); 

    // LED Pulse Amplitude
    max30102_write_reg(REG_LED1_PA, 0x24); // Red
    max30102_write_reg(REG_LED2_PA, 0x24); // IR

    // Mode Config (SpO2 Mode)
    max30102_write_reg(REG_MODE_CONFIG, 0x03);*/

    //cheng setup
    // Reset
    max30102_write_reg(REG_MODE_CONFIG, 0x40);
    // Note: Should wait a bit here in main or use a delay
    __delay_ms(10);
    // FIFO Config
    
    max30102_write_reg(REG_FIFO_WR_PTR, 0x00);
    max30102_write_reg(REG_OVF_COUNTER, 0x00);
    max30102_write_reg(REG_FIFO_RD_PTR, 0x00);

    // SMP_AVE = 4 (010), FIFO_ROLLOVER_EN = 1 (1), FIFO_A_FULL = 0 (0000)
    max30102_write_reg(REG_FIFO_CONFIG, 0x50); 

    // SpO2 Config
    // ADC Range = 4096nA (01), Sample Rate = 400Hz (011), Pulse Width = 411us (11)
    max30102_write_reg(REG_SPO2_CONFIG, 0x2F); 

    // LED Pulse Amplitude
    max30102_write_reg(REG_LED1_PA, 0x0A); // Red dimmed
    max30102_write_reg(REG_LED2_PA, 0x1F); // IR

    // Mode Config (SpO2 Mode)
    max30102_write_reg(REG_MODE_CONFIG, 0x03);

    return true;
}

uint8_t max30102_available(void) {
    uint8_t wr = max30102_read_reg(REG_FIFO_WR_PTR);
    uint8_t rd = max30102_read_reg(REG_FIFO_RD_PTR);
    return (wr - rd) & 0x1F;
}

void max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led) {
    uint32_t red, ir;

    i2c_start();
    i2c_write(MAX30102_ADDR_WRITE);
    i2c_write(REG_FIFO_DATA);
    i2c_rep_start();
    i2c_write(MAX30102_ADDR_READ);

    red  = ((uint32_t)i2c_read(1)) << 16;
    red |= ((uint32_t)i2c_read(1)) << 8;
    red |= i2c_read(1);

    ir   = ((uint32_t)i2c_read(1)) << 16;
    ir  |= ((uint32_t)i2c_read(1)) << 8;
    ir  |= i2c_read(0);

    i2c_stop();

    red &= 0x3FFFF;
    ir  &= 0x3FFFF;

    *pun_red_led = red;
    *pun_ir_led  = ir;
}