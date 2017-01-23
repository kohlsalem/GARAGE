

#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <WiFiUdp.h>
#include <ArduinoOTA.h>    //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA

WiFiUDP listener;
ESP8266WebServer server(80);

#define RELAIS    D1
#define CONTACT   D3



#define RELAISon  digitalWrite(RELAIS,HIGH)
#define RELAISoff digitalWrite(RELAIS,LOW)

void loop() {
  LED_BUILTIN;
  ArduinoOTA.handle();
  server.handleClient();
  delay(50);
}


void setupWebServer(){
 // listen to brightnes changes
 server.on("/", []()
 {
   RELAISon;
   delay(300);
   RELAISoff;
   /*
   if (server.arg("b") != "")
   { int b = constrain((byte) server.arg("b").toInt(),0,100);



 }*/
   String s = "{\n   \"b\":";
   s += 0;
   s += "\n}";

   server.send(200, "text/plain", s);
 });

 // start the webserver
 server.begin();

}

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  pinMode(RELAIS, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CONTACT, INPUT_PULLUP );
WiFiManager wifiManager;


 //fetches ssid and pass from eeprom and tries to connect
 //if it does not connect it starts an access point with the specified name
 //here  "GARAGE"
 //and goes into a blocking loop awaiting configuration
 wifiManager.autoConnect("GARAGE");
 //WiFi.begin(ssid, password);

 // if wlan fails we sit and wait the watchdog ;-)
 while (WiFi.status() != WL_CONNECTED) {

 }
 //set_rgb_all( 0, 0, 100);
 // the pins get initiated for output and swithced off

 pinMode(RELAIS, OUTPUT);
 RELAISon;
 delay(100);
 RELAISoff;

 // initialize OTA update
 // in case, debug output can be placed in the stubs
 ArduinoOTA.setHostname("Candle");
 ArduinoOTA.onStart([](){});
 ArduinoOTA.onEnd([](){});
 ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){});
 ArduinoOTA.onError([](ota_error_t error){});
 ArduinoOTA.begin();

 setupWebServer();

}


/*#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <WiFiUdp.h>
#include <ArduinoOTA.h>    //https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA

//const char* ssid = "Bobby"; // SSID des vorhandenen WLANs
//const char* password = "1234KohlSalem5678"; // Passwort für das vorhandene WLAN

WiFiUDP listener;
ESP8266WebServer server(80);

// RGB FET
#define RELAIS    D1


#define RELAISon  digitalWrite(RELAIS,HIGH)
#define RELAISoff digitalWrite(RELAIS,LOW)

void loop() {

  server.handleClient();
  ArduinoOTA.handle();

  delay(100);


}


void setupWebServer(){
 // listen to brightness changes
 server.on("/", []()
 {

   String s = "{\n   \"b\":";
   s += 0;
   s += "\n}";
   server.send(200, "text/plain", s);

 });
 server.on("/on", []()
 {   Serial.print("On\n");
     RELAISon;
     delay(300);
     RELAISoff;
     String s = "{\n   \"b\":";
     s += 1;
     s += "\n}";
     server.send(200, "text/plain", s);
 });

 // start the webserver
 server.begin();

}

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
 WiFiManager wifiManager;


 // pin setup
 pinMode(RELAIS, OUTPUT);

 //fetches ssid and pass from eeprom and tries to connect
 //if it does not connect it starts an access point with the specified name
 //here  "H801"
 //and goes into a blocking loop awaiting configuration
 wifiManager.autoConnect("GARAGE");


 setupWebServer();


 RELAISoff;
}
*/
