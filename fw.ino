#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "........";
const char* password = "........";

ESP8266WebServer server(80);
// ON-request handler
void handleOn(){
}
//OFF-request handler
void handleOff(){
}

//Setup
void setup(void){
  WiFi.begin(ssid, password);
 // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
 //set dns name 
  if (MDNS.begin("timer")) {;
  }
  
//server handlers
  server.on("/on", handleOn);
  server.on("/off",handleOff));
  server.begin();
}

void loop(void){
  server.handleClient();
}
