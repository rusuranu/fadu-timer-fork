
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#define ledpin 2 // pin to led matrix.
#define ledCount 64 // led count
#define brightness 240 //max brightness for matrix 

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "Password"
#endif
const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
Adafruit_NeoPixel matrix(ledCount, ledpin, NEO_GRB + NEO_KHZ800);

int colorValue(int percent){
  if ((percent>100)or(percent<0)){return 0;};
  return (int) round(2.54*percent);
}

// ON-request handler
void handleOn(){
 server.send(200, "text/plain", ""); 
 if( server.args()>=3){
   int R=colorValue(server.arg("R").toInt());
   int G=colorValue(server.arg("G").toInt());
   int B=colorValue(server.arg("B").toInt());
   if (server.arg("color")=="1"){
     int G=server.arg("G").toInt();
     int B=server.arg("B").toInt();
     for(int i=0; i<matrix.numPixels(); i++) { // For each pixel in matrix..
       if((i+i/8)%2) { matrix.setPixelColor(i, R,G,B);}
       else {matrix.setPixelColor(i, R,0,0);};
     } 
   }
   else{
     matrix.fill(matrix.Color(R,G,B),0,ledCount);
   }
   matrix.show();
 }
 else {
   matrix.fill(matrix.Color(60,60,60),0,ledCount); 
   matrix.show();
 }

}

//OFF-request handler
void handleOff(){
   matrix.clear();
   matrix.show();
   server.send(200, "text/plain", "");
   
}

//Setup
void setup(void){
  //matrix setup
  matrix.begin();
  matrix.setBrightness(brightness);
  matrix.show();
  
  //wifi setrup
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    matrix.fill(matrix.Color(random(60),random(60),random(60)),0,ledCount);
    matrix.show();
    delay(500);
    matrix.clear();
    matrix.show();
  }
  
  //set dns name 
  if (MDNS.begin("timer")) {;}
   

  //server handlers
  server.on("/on", handleOn);
  server.on("/off",handleOff);
  server.begin();
}

void loop(void){
  server.handleClient();
}
