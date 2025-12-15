#include "uart.h"
#include <string.h>
#include <ctype.h>

char uart_buffer[STR_MAX];
int uart_buffer_size = 0;

void uart_initialize(void){
    // Configure UART
    /*
        TODObasic
        Serial Setting
        1.   Setting Baud rate
        2.   choose sync/async mode
        3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
        3.5  enable Tx, Rx Interrupt(optional)
        4.   Enable Tx & RX
    */
    TRISCbits.TRISC6 = 1;  // RC6(TX) : Transmiter set 1 (output)
    TRISCbits.TRISC7 = 1;  // RC7(RX) : Receiver set 1   (input)

    // Setting Baud rate (when fosc = 8M)
    // Baud rate = 115200 (Look up table)
    TXSTAbits.SYNC = 0;     // Synchronus or Asynchronus
    BAUDCONbits.BRG16 = 1;  // 16 bits or 8 bits
    TXSTAbits.BRGH = 1;     // High Baud Rate Select bit
    SPBRG = 16;             // Control the period
    SPBRGH = 0x00;            // Used when BRG16 = 1 

    // Serial enable
    RCSTAbits.SPEN = 1;  // Enable asynchronus serial port (must be set to 1)
    PIR1bits.TXIF = 0;   // Set when TXREG is empty
    PIR1bits.RCIF = 0;   // Will set when reception is complete
    TXSTAbits.TXEN = 1;  // Enable transmission
    RCSTAbits.CREN = 1;  // Continuous receive enable bit, will be cleared when error occured
    PIE1bits.TXIE = 0;   // Wanna use Interrupt (Transmit)
    IPR1bits.TXIP = 0;   // Interrupt Priority bit
    PIE1bits.RCIE = 1;   // Wanna use Interrupt (Receive)
    IPR1bits.RCIP = 1;   // Interrupt Priority bit
    /* Transmitter (output)
        TSR : Current Data
        TXREG : Next Data
        TXSTAbits.TRMT will set when TSR is empty
    */
    /* Reiceiver (input)
        RSR   : Current Data
        RCREG : Correct Data (have been processed) : read data by reading the RCREG Register
    */

}

void uart_clear_buffer() {
    for (int i = 0; i < STR_MAX; i++)
        uart_buffer[i] = '\0';
    uart_buffer_size = 0;
}

void uart_read() {
    char data = RCREG;
    if (!isprint(data) && data != '\r') return;
    uart_buffer[uart_buffer_size++] = data;
    //putch(data);
}

int uart_get_string(char *str) {
    if (uart_buffer[uart_buffer_size - 1] == '\r') {
        uart_buffer[--uart_buffer_size] = '\0';
        strcpy(str, uart_buffer);
        uart_clear_buffer();
        return 1;
    } else {
        str[0] = '\0';
        return 0;
    }
}

void putch(char data) {  // Output on Terminal
    if (data == '\n' || data == '\r') {
        while (!TXSTAbits.TRMT);
        TXREG = '\r';
        while (!TXSTAbits.TRMT);
        TXREG = '\n';
    } else {
        while (!TXSTAbits.TRMT);
        TXREG = data;
    }
}
