#include <xc.h>
#include "uart.h"

/**
 * @brief initialize UART module, baud_rate=1200
 */
void UART_Initialize() {
           
    /**
     * Serial Setting      
     *  1.   Setting Baud rate
     *  2.   choose sync/async mode 
     *  3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     *  3.5  enable Tx, Rx Interrupt(optional)
     *  4.   Enable Tx & RX
     */
    
    // set TX and RX pins as 1
    TRISCbits.TRISC6 = 1;            
    TRISCbits.TRISC7 = 1;
    
    //  Serial enable
    RCSTAbits.SPEN = 1; // TX/RX as serial     
    
    //  ========== Setting baud rate ==========
    TXSTAbits.SYNC = 0;         // async
    BAUDCONbits.BRG16 = 0;      // 8-bit     
    TXSTAbits.BRGH = 0;
    SPBRG = 12;                 // see table to set SPBRGH:SPBRG
    // ========================================
    
    
    TXSTAbits.TXEN = 1; // enable TX           
    RCSTAbits.CREN = 1; // enable RX
    
    PIE1bits.TXIE = 0;  // disable TX interrupt       
    IPR1bits.TXIP = 0;  // TX interrupt priority 
    PIR1bits.TXIF = 0;  // indicate TXREG is empty initially
    
    PIE1bits.RCIE = 0;  // enable RX interrupt
    IPR1bits.RCIP = 0;  // RX interrupt priority: low
    PIR1bits.RCIF = 0;  // set when reception is complete
}

/**
 * @brief Output to receiver
 * @param data: an 8-bit element to be transfered
 */
void UART_Write(unsigned char data) {
    while(!TXSTAbits.TRMT);
    TXREG = data;
}

/**
 * @brief Output to receiver
 * @param text: an array of 8-bit elements to be transfered
 */
void UART_Write_Text(char* text) {
    for(int i=0;text[i]!='\0';i++)
        UART_Write(text[i]);
}