#include <ESP8266HTTPClient.h>

/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "WLAN-359675";
const char* password = "Horst&Christiane";
const char* host = "192.168.2.200";


void setup () {
  delay(90000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}
 
void loop() {
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
    HTTPClient http;  //Declare an object of class HTTPClient
 
    http.begin("http://192.168.2.200:80/api");  //Specify request destination
    int httpCode = http.GET();           //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end();   //Close connection
 
  }
 
  delay(2000);    //Send a request every 2 seconds
 
}
