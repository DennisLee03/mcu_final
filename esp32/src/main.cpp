#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <math.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include "calculate.h"
#include "heart_rate.h"


const char* SSID  = "DennisPhone";        // @TODO: WiFi's name
const char* PASS  = "dennis0928";         // @TODO: WiFi's password
const char* PC_IP = "172.20.10.7";        // @TODO: PC's IPv4

WebServer server(80);

bool data_ready = 0;
String data_str;
long alcohol;
long ir;

/**
 * @brief HTTP request's hanlder
 */
void sendData(){
    StaticJsonDocument<300> JSONData;
    // Use the object just like a javascript object or a python dictionary  
    JSONData["AC"] = alcohol; 
    JSONData["HR"] = beatAvg;  
    JSONData["HRV"] = rmssd; 
    // You can add more fields
    char data[300];

    // Converts the JSON object to String and stores it in data variable
    serializeJson(JSONData, data);

    // Set content type as application/json and send the data
    server.send(200, "application/json", data);
}

/**
 * @brief Connect to WiFi
 */
void connect_wifi() {
  /// Initialize wifi 
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting Wi-Fi...");
  }
  Serial.println("Wi-Fi Successfully Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  server.on("/test", HTTP_GET, sendData);
  server.begin();

  delay(100);
}


void setup() {
  Serial.begin(115200);
  connect_wifi();
}

void loop() {

  server.handleClient();

  // byte-wise read data
  while(Serial.available()) {
    /**
     * read <IR>,<AC> from PIC18F4520
     */
    char ch = Serial.read();
    if(ch == ',') {
      ir = data_str.toInt();
      data_str = "";
    } else if(ch == '\n') {
      alcohol = data_str.toInt();
      data_ready = true;
      data_str = "";
      break;
    } else {
      if(isDigit(ch)) data_str += ch;
    }
  }

  if(data_ready) {
    calc_rmssd(ir);
    Serial.printf("IR: %ld, AC: %ld, HR: %ld, HRV: %f\r\n", ir, alcohol, beatAvg, rmssd);
    data_ready = false;
  }
}
