# mcu_final

## Overview
* `esp32`: As a WiFi module, a connection between pic18f4520 and PC.
* `pc`: Python scripts on PC. Interpret data and visuialize it.
* `pic.X`: pic18f4520's code.

## Hardware Components
1. `PIC18F4520`: contorller of MQ-3 and MAX30102
2. `ESP32 DevKit V1`: WiFi module
3. `MQ-3`: measure alcohol
4. `MAX30102`: measure heart rate

## Development Environment
1. `MPLAB` for pic18
2. `PlatformIO` for esp32

## Reminders
* If there is "**Target Device ID (0x0) is an Invalid Device ID. Please check your connections to the Target Device.**" when compiling:
    1. Your breadboard is probably broken!
    2. Check that the pic18’s pins are fully inserted.
* In `mcu_final/esp32/src/main.cpp`, change below when in different WiFi environments. Then re-burn the program to the esp32.
    ```
    const char* SSID  = "dennis-laptop";   // @TODO: WiFi's name
    const char* PASS  = "32170928";        // @TODO: WiFi's password
    const char* PC_IP = "192.168.137.1";   // @TODO: PC's IPv4
    ```

## PIC18's Config
1. $$F_{OSC}$$ = 1 MHz
    * It is possible that if we try to integrate I2C into our project, we may adjust ouut $$F_{OSC}$$.
    * Therefore, some modules's config need changing
        1. ADC: $$T_{AD}$$, $$T_{ACQT}$$
        2. UART: `SYNC`, `BRG16`, `BRGH`, `SPBRGH:SPBRG`
3. baud_rate = 1200 bps

## TODOs
- [ ] process data
    - [ ] interpret heart-rate and breath-test data
    - [ ] how to display them
- [ ] handle heart-rate and breath-test modes
- [ ] power supply
- [ ] I2C comm of MAX30102