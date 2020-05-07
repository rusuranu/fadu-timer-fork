/* DIY Enlarger Color Head firmware for usage with Android Fadu darkroom timer app.
 * and COB LEDs under PWM driver
 * 
 * 
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#define Rpin D4 // PWM pin to red chanel
#define Gpin D2 // PWM pin to green  chanel
#define Bpin D3 // PWM pin to blue chanel
#define Fpin D6  //fan pin
#define MaxPWM 1023 // max pwm duty cycle 

//fan speed controle. Speed in %
void fan(int speed){
  analogWrite(Fpin,colorValue(speed));
}
// returns pwm value from % of  brigtness
int colorValue(int percent) {
  if ((percent > 100) or (percent < 0)) {
    return 0;
  };
  return (int) round(MaxPWM/100 * percent);
}

//set LED brightness. R G B in %
void setLed(int R, int G, int B)
{
  analogWrite(Rpin,MaxPWM-colorValue(R));
  analogWrite(Gpin,MaxPWM-colorValue(G));
  analogWrite(Bpin,MaxPWM-colorValue(B));
}


//#ifndef STASSID
//#define STASSID "ledEnlarger"
//#define STAPSK  "meopta123"
//#endif
int R;
int G;
int B;

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 5);
IPAddress subnet(255, 255, 255, 0);
//const char* ssid     = STASSID;
//const char* password = STAPSK;

ESP8266WebServer server(80);
//Adafruit_NeoPixel matrix(ledCount, ledpin, NEO_GRB + NEO_KHZ800);



// ON-request handler
void handleOn() {
  server.send(200, "text/plain", "");
  if ( server.args() >= 3) {
      R = server.arg("R").toInt();
      G = server.arg("G").toInt();
      B = server.arg("B").toInt();
      setLed(R,G,B);
    

  }
  else {
    setLed(40,40,40);

  }


}


//OFF-request handler
void handleOff() {
  server.send(200, "text/plain", "");
  setLed(0,0,0);

}

// reset all wifi settings
void resetWifi() {
  server.send(200, "text/plain", "");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.reset();
}

//page for IP auto detection in android app
void locateApp() {
  server.send(202, "text/plain", "");
}


//Setup
void setup(void) {
  setLed(0,0,0);
  fan(50);
  WiFi.softAP("enlarger");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  //  while (WiFi.softAPgetStationNum() < 1)
  for (int i = 0; i < 30; i++)
  {
    setLed(random(10), random(10), random(10));
     delay(500);
    if (WiFi.softAPgetStationNum()) {
      setLed(0,0,0);
      break;
    }
  }
  setLed(0,0,0);
  if (WiFi.softAPgetStationNum() == 0)
  {
    WiFiManager wifiManager;
    wifiManager.autoConnect("enlarger");
  }


  WiFi.hostname("enlarger");
  MDNS.begin("enlarger");
  //server handlers
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/reset", resetWifi);
  server.on("/locate", locateApp);
  server.begin();



}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
