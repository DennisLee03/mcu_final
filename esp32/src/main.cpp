#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>


const char* SSID  = "我叫小phone今年九歲";   // @TODO: WiFi's name
const char* PASS  = "welcome home.";         // @TODO: WiFi's password
const char* PC_IP = "10.26.16.128";       // @TODO: PC's IPv4

const uint16_t PC_PORT = 5000;          // PC listener's port
WiFiClient client;

bool data_ready = 0;
String data_str;

/**
 * @brief Connect to WiFi
 */
void connect_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // 0.5 s
  }
}

/**
 * @brief Ensure TCP connection is made
 */
void ensure_tcp() {
  if (client.connected()) return;
  client.stop();
  client.connect(PC_IP, PC_PORT);
}

/**
 * @brief Sending data to PC using TCP or UDP
 * @todo implenment sending
 */
void send_data() {
  long data = data_str.toInt();

  // ========== SEND ===========
  ensure_tcp();
  if(client.connected()) {
    client.printf("%d", data);
  }
  // ===========================

  data_str = "";
  data_ready = false;
}

void setup() {
  Serial.begin(1200);
  connect_wifi();
  ensure_tcp();
}

void loop() {

  // byte-wise read data
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

  // send data to PC
  if(data_ready) {
    send_data();
  }
}
