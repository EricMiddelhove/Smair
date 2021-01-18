/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <ESP8266WiFiMulti.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;
char id[37];
char stat = '0';

void setup() {

  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WLAN-U4WRB7", "4234354542018221");

  delay(2500);
}

void sendHeyThere(){
   // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    
    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://192.168.2.115:4000/heyThere/")) {  // HTTP


      //Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();

          byte i = 0;

          for(int j = 0; j < 3; j++){
            while(payload.charAt(i) != '"'){
              i++;
            }
            i++;
          }

          byte beginIndex = i;
          while(payload.charAt(i) != '"'){
            id[i - beginIndex] = payload.charAt(i);
            i++;
          }
          
          
         //Serial.println(payload);
          Serial.println(id);
        }
      } else {
        //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      //Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}

void updateStatus(){
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;

    HTTPClient http;

    //Serial.print("[HTTP] begin...\n");
    
    http.begin(client, "http://192.168.2.115:4000/updateStatus/");
    http.addHeader("Content-Type", "application/json");

    String payload = "{ \"ID\" :\"";
    payload.concat(id);
    payload.concat("\", \"status\" : ");
    payload.concat(stat);
    payload.concat("}");
    Serial.print(payload);
    int httpCode = http.POST(payload);
  

    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
     

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String p = http.getString();

        Serial.println(p);
      }
     } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
     }
     http.end();
    }

    
}

void loop() {
  if(Serial.available()){

    String s = Serial.readString();
    
    if(s == "I"){
      sendHeyThere();
    }else if(s.charAt(0) == 'D' && s.charAt(1) == ':'){

      stat = s.charAt(2);
      
      updateStatus();
    }
  }
}
