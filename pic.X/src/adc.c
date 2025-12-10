#include <xc.h>
#include "adc.h"

/**
 * @brief Configure the ADC module, assume Fosc = 1 MHz
 */
void ADC_config() {
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0b1110;   // AN0 as input port
    ADCON0bits.CHS = 0b000;     // AN0 as analog channel
    ADCON2bits.ADCS = 0b000;    // Tosc = 1 us > 0.7 us, therefore select Tad =  2*Tosc
    ADCON2bits.ACQT = 0b001;    // Tacqt = 4 us > 2.4 us
    ADCON2bits.ADFM = 1;        // left-justified(8-bit), read ADRESH
    ADCON0bits.ADON = 1;        // turn it on
    
    return;
}

/**
 * @brief Configure the ADC interrupt
 */
void ADC_int_config() {
    PIE1bits.ADIE = 0;
    PIR1bits.ADIF = 0;
    IPR1bits.ADIP = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    return;
}

/**
 * @brief Read ADC value using AN0
 * @return 10-bit ADC value
 */
int ADC_Read() {
    int digital;
    
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(GO_nDONE==1); // ADC in progress

    digital = (ADRESH<<8) + (ADRESL);
    return digital;
}