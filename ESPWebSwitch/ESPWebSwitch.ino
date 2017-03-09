#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define REQ_BUF_SZ   60

WiFiServer server(80);

const byte mainPin = 2;
String readString;
char req_index = 0;
char HTTP_req[REQ_BUF_SZ] = {0};

void setup() {
    pinMode(mainPin, OUTPUT);
    Serial.begin(115200);
    WiFiManager wifiManager;
    //wifiManager.resetSettings();
    wifiManager.autoConnect("AutoConnectAP");
    Serial.print("conneced to " + WiFi.SSID() + " WiFi network!");
    //Serial.println();
    server.begin();
    digitalWrite(mainPin, HIGH);
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString = readString + c;// Store characters to string
        }
        if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
        if (c == '\n' && currentLineIsBlank) {
          client.println(F("http/1.1 200 ok"));// Send standard http headers
          client.println(F("content-type: text/html"));
          client.println();
          client.println(F("<!doctype html><html>"));
          client.println(F("<body bgcolor='brown'>"));
          client.println(F("<center><head><title>Home Web Switch Control</title></head>"));
          client.println(F("<h2>Home WebSwitch</h2>"));
          client.println(F("<h4>Made by covek</h4>"));
          client.println(F("<hr/><p> Click the Buttons to turn On and OFF <p/><hr/>"));
          client.print(F("<input type=button value='ON' onmousedown=location.href='/?_on'>"));
          client.println(F("<input type=button value='OFF' onmousedown=location.href='/?_off'><br/><br/>"));
          client.println(F("</body></html>"));
          delay(1);// Page loading delay
          client.stop();// Stopping client
          if (readString.indexOf("/?_on") > 0 ) {
            digitalWrite(mainPin, LOW);
          }
          if(readString.indexOf("/?_off") > 0) {
            digitalWrite(mainPin, HIGH);
          }
          readString = "";// Clearing string for next read
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
  }
}
