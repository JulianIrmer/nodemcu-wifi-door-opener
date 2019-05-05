#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
 
ESP8266WebServer server(80);
const char* ssid = "Garage";
const char* password = "Horst&Christiane";
const int pinRelais = D4;
const int pinReed = D5;
bool doorClosed;
String doorStatus;

void setup() {
  pinMode(pinRelais, OUTPUT);
  pinMode(pinReed, INPUT_PULLUP);
 
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  IPAddress ip(192,168,2,200);
  IPAddress gateway(192,168,2,1);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
 
  server.on("/api", handleRequest);
  server.on("/manu", manualDoor);

  server.on("/doorStatus", []() {   //Define the handling function for the path
    if(doorClosed == false){
      server.sendHeader("Access-Control-Max-Age", "10000");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
      server.send(200, "text / plain", "open");  
    }
    if(doorClosed == true){
      server.sendHeader("Access-Control-Max-Age", "10000");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
      server.send(200, "text / plain", "closed");
    }
  });
 
  server.begin();                    //Start the server
  Serial.println("Server listening");
}

void loop() {
  showStatus();
  digitalWrite(pinRelais, HIGH);
  if(digitalRead(pinReed) == 1){
    doorClosed = false;
    doorStatus = "Garage offen";
  }
  if(digitalRead(pinReed) == 0){
    doorClosed = true;
    doorStatus = "Garage geschlossen";
  }
  server.handleClient();         //Handling of incoming requests
}
 
void showStatus(){
  Serial.println(digitalRead(pinReed));
  Serial.println();
  delay(500);
}

void handleRequest(){
  server.send(200, "text / plain", "Received Request");
  Serial.println("Request received!");
    
  if(doorClosed == true){
    server.send(200, "text / plain", "Opening Door");
    Serial.println("Door is opening!");
    digitalWrite(pinRelais, LOW);
    delay(1000);
    digitalWrite(pinRelais, HIGH);
  }
  else{
    server.send(200, "text / plain", "Door already open");
  }
}

void manualDoor(){
  server.sendHeader("Access-Control-Max-Age", "10000");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "text / plain", "Received Request");
  Serial.println("Request received!");
  digitalWrite(pinRelais, LOW);
  delay(1000);
  digitalWrite(pinRelais, HIGH);
}


