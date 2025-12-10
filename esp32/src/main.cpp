#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial my_uart(0);

void setup() {
  my_uart.begin(1200);
}

void loop() {
  my_uart.println("Hello ESP32!");
  delay(1000);
}
