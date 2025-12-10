#include <Arduino.h>
#include <HardwareSerial.h>


bool data_ready = 0;
String data_str;

void setup() {
  Serial.begin(1200);
}

void loop() {

  // byte-wise get data
  while(Serial.available()) {
    char ch = Serial.read();
    if(ch == '\n') {
      data_ready = true;
      break;
    } else {
      if(isDigit(ch)) {
        data_str += ch;
      }
    }
  }

  // getting data finish
  if(data_ready) {
    long data = data_str.toInt();    // parse int (non-numeric -> 0)
    Serial.print(F("ADC_Val = "));
    Serial.println(data);
    data_str = "";
    data_ready = false;

    // @todo transmit data by WiFi
  }
}
