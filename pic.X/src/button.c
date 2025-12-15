#include "button.h"

void button_initialize(void){
    INTCONbits.INT0IF = 0;  // Clear INT0 flag
    INTCONbits.INT0IE = 1;  // Enable INT0 interrupt
    //INTCON2bits.INTEDG0 = 1; // rising edge triggered (pressed)
    TRISB |= 0x01;      // RB0 as input for button
}