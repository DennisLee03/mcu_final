#include <xc.h>
#include <pic18f4520.h>
#include "i2c_master.h"

/**
 * @brief Check whether I2C is idle
 * In I2C Master mode, does not allow queueing of events.
 */
void I2C_wait_idle(void) {
    while((SSPCON2 & 0x1F) || SSPSTATbits.R_nW);
    // check ACKEN, RCEN, PEN, RSEN, SEN || whether transmit is in progress
    // when these conditions are 0, we can do next instruction
}

/**
 * @brief Init start condition
 */
void I2C_start(void) {
    I2C_wait_idle();
    SSPCON2bits.SEN = 1;
}

/**
 * @brief Init repeated start condition
 */
void I2C_rep_start(void) {
    I2C_wait_idle();
    SSPCON2bits.RSEN = 1;
}

/**
 * @brief Init stop condition
 */
void I2C_stop(void) {
    I2C_wait_idle();
    SSPCON2bits.PEN = 1;
}

/**
 * @brief Send data to slave
 * @param data
 */
void I2C_write(uint8_t data) {
    I2C_wait_idle();
    SSPBUF = data;
    while(SSPSTATbits.BF);      // BF=1, buffer is full
    while(SSPCON2bits.ACKSTAT); // ACKSTAT=1, ACK was NOT received from slave
}

/**
 * @brief Read data from slave
 * @param ack to initiates Acknowledge sequence
 * @return received data from slave
 */
uint8_t I2C_read(uint8_t ack) {
    uint8_t received = 0;
    I2C_wait_idle();
    SSPCON2bits.RCEN = 1;
    while(!SSPSTATbits.BF); // wait for SSPBUF full
    received = SSPBUF;
    SSPCON2bits.ACKEN = ack;
    return received;
}

/**
 * @brief Init I2C module
 */
void I2C_init(void) {
    TRISCbits.RC3 = 1;      // serial port: SCL
    TRISCbits.RC4 = 1;      // serial port: SDA
    SSPSTATbits.SMP = 1;    // standard speed: 100 kHz or 1MHz
    SSPSTATbits.CKE = 0;    // disable SMBus
    SSPCON1bits.SSPM = 8;   // master mode
    SSPADD = 19;            // @todo adjust Fosc and this
    SSPCON1bits.SSPEN = 1;  // enable serial ports: SCL, SDA
}