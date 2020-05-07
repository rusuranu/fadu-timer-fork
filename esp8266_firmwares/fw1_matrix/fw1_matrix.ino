/* DIY Enlarger Color Head firmware for usage with Android Fadu darkroom timer app.
 * 
 * 
 * 
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#define ledpin D2 // pin to led matrix.
#define ledCount 64 // led count
#define brightness 240 //max brightness for matrix 


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
Adafruit_NeoPixel matrix(ledCount, ledpin, NEO_GRB + NEO_KHZ800);

int colorValue(int percent) {
  if ((percent > 100) or (percent < 0)) {
    return 0;
  };
  return (int) round(2.54 * percent);
}

// ON-request handler
void handleOn() {
  server.send(200, "text/plain", "");
  if ( server.args() >= 3) {
    if (server.arg("color") == "1") {
      R = colorValue(server.arg("R").toInt());
      G = server.arg("G").toInt();
      B = server.arg("B").toInt();
      for (int i = 0; i < 8; i++) { // For each pixel in matrix..
        for (int j = 0; j < 8; j++) {
          if ((i + j * 8) % 3 == 0) {
            matrix.setPixelColor(j + 8 * i, R, G, B);
          }
          else {
            matrix.setPixelColor(i * 8 + j, R, 0, 0);
          };
        }

      }
    }
    else {
      R = colorValue(server.arg("R").toInt());
      G = colorValue(server.arg("G").toInt());
      B = colorValue(server.arg("B").toInt());
      matrix.fill(matrix.Color(R, G, B), 0, ledCount);
    }

  }
  else {
    matrix.fill(matrix.Color(60, 60, 60), 0, ledCount);

  }
  matrix.show();


}

//OFF-request handler
void handleOff() {
  server.send(200, "text/plain", "");
  matrix.clear();
  matrix.show();

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
  Serial.begin(115200);
  //matrix setup
  matrix.begin();
  matrix.setBrightness(brightness);
  matrix.show();
  //wifiManager.setAPStaticIPConfig(IPAddress(10, 0, 1, 1), IPAddress(10, 0, 1, 10), IPAddress(255, 255, 255, 0));


  //WiFiManager wifiManager;
  //wifiManager.resetSettings();

  //  wifi setrup
  //WiFi.mode(WIFI_STA);
  //WiFi.begin(ssid, password);

  WiFi.softAP("enlarger");
  WiFi.softAPConfig(local_IP, gateway, subnet);
  //  while (WiFi.softAPgetStationNum() < 1)
  for (int i = 0; i < 30; i++)
  {
    matrix.fill(matrix.Color(random(30), random(10), random(10)), 0, ledCount);
    matrix.show();
    delay(500);
    matrix.clear();
    matrix.show();
    if (WiFi.softAPgetStationNum()) {
      break;
    }
  }
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
