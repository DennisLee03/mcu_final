#include <xc.h>
#include "int.h"

/**
 * @brief init global interrupt config
 */
void INTERRUPT_Initialize () {
    RCONbits.IPEN = 1;       // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;     // enable high priority interrupt
    INTCONbits.GIEL = 1;     // enable low priority interrupt
}

/**
 * @brief init INT0 
 */
void INT0_init() {
    ADCON1 = 0x0E;           // only RA0/AN0 is analog pin in our convention
    TRISBbits.RB0 = 1;       // RB0/INT0 as input pin
    INTCONbits.INT0F = 0;    // flag=0
    INTCONbits.INT0E = 1;    // enable INT0 
    INTCON2bits.INTEDG0 = 1; // leading edge
}