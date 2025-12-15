#include "adc.h"

void adc_initialize(void){
    // Configure ADC
    TRISAbits.RA0 = 1;         // Set RA0 as input port
    ADCON0bits.CHS = 0b0000;   // Select AN0 channel
    ADCON1bits.VCFG0 = 0;      // Vref+ = Vdd
    ADCON1bits.VCFG1 = 0;      // Vref- = Vss
    ADCON2bits.ADCS = 0b101;   // ADC clock Fosc/16 Tad = 2us
    ADCON2bits.ACQT = 0b001;   // 2Tad acquisition time 4us
    ADCON0bits.ADON = 1;       // Enable ADC
    ADCON2bits.ADFM = 1;       // Right justified
    PIR1bits.ADIF = 0;         // Clear ADC flag
    IPR1bits.ADIP = 0;         // Set ADC interrupt priority to low
    PIE1bits.ADIE = 1;         // Enable ADC interrupt
}

void adc_start_conversion(void) {
    if (ADCON0bits.GO == 0) {
        ADCON0bits.GO = 1;
    }
}

