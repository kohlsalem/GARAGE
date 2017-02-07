

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

const char* openhab_host = "192.168.178.38";

// Door runs for 20 seconds
#define RUN_TIMEOUT 23000

#define STATE_OPEN 2
#define STATE_CLOSING 3
#define STATE_CLOSING_RETRY 4
#define STATE_ERROR 5
#define STATE_CLOSED 0
#define STATE_OPENING 1

int currentState = STATE_OPEN;
unsigned long stateChagedTime = millis();
int currentSensor = HIGH; //initialoze to open. If it is actually closed on startup, it will update the state directly

void pressButton(){
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(RELAIS,HIGH);
  delay(300);
  digitalWrite(RELAIS,LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}


void updateState(int newState){
  String s = "";
  String value = "";
  String vlength = "";

  //Change the state
  currentState = newState;

  //remember the timestamp for the timeouts
  stateChagedTime = millis();

  switch(currentState){
    case STATE_OPEN:          value = "OPEN";    vlength = "4"; break;
    case STATE_CLOSING:       value = "CLOSING"; vlength = "7"; break;
    case STATE_CLOSING_RETRY: value = "RETRY";   vlength = "5"; break;
    case STATE_ERROR:         value = "ERROR";   vlength = "5"; break;
    case STATE_CLOSED:        value = "CLOSED";  vlength = "6"; break;
    case STATE_OPENING:       value = "OPENING"; vlength = "7"; break;
  }


  WiFiClient client; // Webclient initialisieren
  if (!client.connect(openhab_host, 8080)) { // connect with openhab 8080
    Serial.println("Fehler: Verbindung zur OH konnte nicht aufgebaut werden");
     delay(100);
    return;
  }
  Serial.println("Sending"+value);
  client.println("PUT /rest/items/Hoermann/state HTTP/1.1");
  client.print("Host: ");
  client.println(openhab_host);
  client.println("Connection: close");
  client.println("Content-Type: text/plain");
  client.println("Content-Length: "+vlength+"\r\n");
  client.print(value);

}


void doOpen(){

 // we open the door only if it was closed. Actually, we can be very sure about that...
 if( currentState == STATE_CLOSED){
   // the state update should come form the sensor
   pressButton();
 }
}

void doClose(){

 // we close only from open or error state
 if((currentState == STATE_OPEN)||
    (currentState == STATE_ERROR)){

    updateState(STATE_CLOSING);
    pressButton();
 }
}

void loop() {
  int newSensor;
  unsigned long stateChangeElapsed = millis()-stateChagedTime;

  newSensor = digitalRead(CONTACT);
/*  Serial.print(currentSensor);
  Serial.print(" --> ");
  Serial.println(newSensor);*/
  if(currentSensor == HIGH && newSensor == LOW){
     // no matter what state: the door just closed.
     updateState(STATE_CLOSED);
  } else if(currentSensor == LOW && newSensor == HIGH){
     // no matter what state: the door is just opening
     updateState(STATE_OPENING);
  } else if( (currentState == STATE_OPENING)&&(stateChangeElapsed>RUN_TIMEOUT )) {
     // after 20 sec opening, we consider it open
     updateState(STATE_OPEN);
  } else if( (currentState == STATE_CLOSING)&&(stateChangeElapsed>RUN_TIMEOUT )) {
     // after 20 sec closing & still not closed? Retry!
     updateState(STATE_CLOSING_RETRY);
     pressButton();
  } else if( (currentState == STATE_CLOSING_RETRY)&&(stateChangeElapsed>RUN_TIMEOUT )){
     // after 20 sec of retry still not work --> Error. Somebody messed around with the HW Button
     updateState(STATE_ERROR);
  }
  currentSensor = newSensor;
  ArduinoOTA.handle();
  server.handleClient();
  delay(100);
}


void setupWebServer(){
 // listen to brightnes changes
 server.on("/", []()
 {
   /*
   if (server.arg("b") != "")
   { int b = constrain((byte) server.arg("b").toInt(),0,100);



 }*/
   String s = "{\n   \"state\":";
   s += currentState;
   s += "\n}";

   server.send(200, "text/plain", s);
 });
 server.on("/open", []()
 {
   doOpen();

   String s = "{\n   \"open - state\":";
   s += currentState;
   s += "\n}";

   server.send(200, "text/plain", s);
 });
 server.on("/close", []()
 {
   doClose();

   String s = "{\n   \"close - state\":";
   s += currentState;
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

  digitalWrite(LED_BUILTIN, LOW);
  
 // Set configuration portal time out  - for 3 mins ( enough?)
  wifiManager.setConfigPortalTimeout(180);
 //fetches ssid and pass from eeprom and tries to connect
 //if it does not connect it starts an access point with the specified name
 //here  "GARAGE"
 //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect("GARAGE")) {
    //if it is not able to connect - restart it and try again :)
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  } 
 //WiFi.begin(ssid, password);

 // if wlan fails we sit and wait the watchdog ;-)
 while (WiFi.status() != WL_CONNECTED) {
   delay(150);
   digitalWrite(LED_BUILTIN, HIGH);
   delay(150);
   digitalWrite(LED_BUILTIN, LOW);
 }
 digitalWrite(LED_BUILTIN, HIGH);
 // initialize OTA update
 // in case, debug output can be placed in the stubs
 ArduinoOTA.setHostname("GARAGE");
 ArduinoOTA.onStart([](){});
 ArduinoOTA.onEnd([](){});
 ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){});
 ArduinoOTA.onError([](ota_error_t error){});
 ArduinoOTA.begin();

 setupWebServer();

}
