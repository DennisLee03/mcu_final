#include "i2c.h"
#include "config.h"


void i2c_initialize(void) 
{
    /*
    //100k
    TRISCbits.RC3 = 1;      // serial port: SCL
    TRISCbits.RC4 = 1;      // serial port: SDA
    SSPSTATbits.SMP = 1;    // standard speed: 100 kHz or 1MHz
    SSPSTATbits.CKE = 1;    // disable SMBus
    SSPCON1bits.SSPM = 8;   // master mode
    SSPADD = 19;            // @todo adjust Fosc and this speed: 100khz
    SSPCON1bits.SSPEN = 1;  // enable serial ports: SCL, SDA
    */
    //200k   
    TRISCbits.RC3 = 1;      // serial port: SCL
    TRISCbits.RC4 = 1;      // serial port: SDA
    SSPSTATbits.SMP = 0;    // high speed 
    SSPSTATbits.CKE = 1;    // disable SMBus
    SSPCON1bits.SSPM = 8;   // master mode
    SSPADD = 4;            // @todo adjust Fosc and this speed: 400khz
    SSPCON1bits.SSPEN = 1;  // enable serial ports: SCL, SDA

}

void i2c_is_idle(void)
{
    while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_W));
}

void i2c_start(void)
{
    i2c_is_idle();
    SSPCON2bits.SEN = 1;
}

void i2c_rep_start(void)
{
    i2c_is_idle();
    SSPCON2bits.RSEN = 1;
}

void i2c_stop(void)
{
    i2c_is_idle();
    SSPCON2bits.PEN = 1;
}

void i2c_write(uint8_t i2c_data)
{
    i2c_is_idle();
    SSPBUF = i2c_data;
    while(SSPSTATbits.BF != 0);
    while(SSPCON2bits.ACKSTAT != 0);
}

uint8_t i2c_read(uint8_t ack)
{
    uint8_t recieve = 0;
    i2c_is_idle();
    SSPCON2bits.RCEN = 1;
    while(SSPSTATbits.BF != 1);
    recieve = SSPBUF;
    SSPCON2bits.ACKEN = ack;
    return recieve;
} 