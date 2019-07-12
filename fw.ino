#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#define ledpin 6 // pin to led matrix.
#define ledCount 64 // led count
#define brightness 130 //max brightness for matrix 

const char* ssid = "........";
const char* password = "........";

ESP8266WebServer server(80);
Adafruit_NeoPixel matrix(ledcount, ledpin, NEO_GRB + NEO_KHZ800);

//fillcolor
void fillCollor(int R, int G, int B) {
  for(int i=0; i<matrix.numPixels(); i++) { // For each pixel in matrix...
    matrix.setPixelColor(i, R,G,B);         //  Set pixel's color (in RAM)
    matrix.show();                          //  Update matrix to match
  }
}
int colorValue(percent){
  if ((percent>100)or(percent<0)){return 0;};
  return (int) round(2.54*percent);
  
}

// ON-request handler
void handleOn(){
 if( server.args()=3){
   int R=colorValue(server.arg('R').toInt());
   int G=colorValue(server.arg('G').toInt());
   int B=colorValue(server.arg('B').toInt());
   //fillCollor(R,G,B);
   matrix.fill(matrix.Color(R,G,B),0,ledCount);
 }
 //else {fillCollor(254,254,254);} 
 else {matrix.fill(matrix.Color(250,250,250),0,ledCount);}
 //martix.show(); 
}

//OFF-request handler
void handleOff(){
  matrix.clear();
}

//Setup
void setup(void){
  //matrix setup
  matrix.begin();
  matrix.show();
  matrix.setBrightness(brightness);
  //wifi setrup
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
