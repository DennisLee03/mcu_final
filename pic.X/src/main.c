#include "config.h"
#include "uart.h"
#include "i2c.h"
#include "adc.h"
#include "button.h"
#include "max30102.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全域變數
volatile int alcohol = 0;
volatile int mode = 0;
volatile bool btn_interr = false;

void system_initialize(void);
void button_pressed(void);
void adc_changed(int value);

void __interrupt(high_priority) H_ISR(void) {

    if (INTCONbits.INT0IF) {  // Handle button interrupt
        INTCONbits.INT0IE = 0; 
        __delay_ms(20);  // bouncing problem
            
        if(PORTBbits.RB0 == 1){
            button_pressed();
        }
        
        btn_interr = true;
        INTCONbits.INT0IF = 0;
        INTCONbits.INT0IE = 1;  
    }

    if (RCIF) {
        if (RCSTAbits.OERR) {
            CREN = 0;
            Nop();
            CREN = 1;
        }

        uart_read();
    }
}

void __interrupt(low_priority) Lo_ISR(void) {
    if (PIR1bits.ADIF) {  // Handle variable register interrupt
        int value = (ADRESH << 8) + ADRESL;
        adc_changed(value);
        PIR1bits.ADIF = 0;
        __delay_ms(5);    // Larger then 2tad
    }

    // process other interrupt sources here, if required
    return;
}

void system_initialize(void) {
    OSCCONbits.IRCF = 0b111;  // Osc = 8 MHz
    ADCON1bits.PCFG = 0b1110;  // AN0 as analog input

    adc_initialize();
    button_initialize();
    uart_initialize();
    i2c_initialize();

    // Configure interrupts
    INTCONbits.PEIE = 1;    // Enable peripheral interrupt
    RCONbits.IPEN = 1;      // enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    // enable high priority interrupt
    INTCONbits.GIEL = 1;    // enable low priority interrupt
    INTCONbits.GIE = 1;     //Enable Global Interrupt
}

// --------------- TODO ------------------

void button_pressed(void) {
    // Do sth when the button is pressed
    /* Example:
    * set_LED(get_LED() + 1);
    */

    mode = !mode;
    alcohol = 0;
}

void adc_changed(int value) {  // value: 0 ~ 1023
    // Do sth when the variable register changes
    /* Example:
    * set_servo_angle(VR_value_to_servo_angle(value));
    * set_LED_analog(VR_value_to_LED_analog(value));
    * printf("%d\n", value); // print the variable register value on uart terminal
    */


    alcohol = value;

}

void main() {
    system_initialize();

    printf("\033[2J\033[H"); // Clear Terminal
    printf("System initializing...\n");

    __delay_ms(100); // Wait for sensor power up
    
    if (max30102_initialize()) {
        printf("MAX30102 Online!\r\n");
    } else {
        printf("MAX30102 Error!\r\n");
        while(1); // Stop here if sensor fails
    }
    
    uint32_t red_val = 0;
    uint32_t ir_val = 0;

    while (1) {
       // 讀取 MAX30102
        uint8_t samples = max30102_available();
        while (samples > 0) {
            max30102_read_fifo(&red_val, &ir_val);
            printf("%lu,%d\n", ir_val, alcohol);
            samples--;
        }
        // 觸發下一次 ADC 轉換 (如果模式為 1)
        if (mode == 1) {
            adc_start_conversion();
        }
    }
}