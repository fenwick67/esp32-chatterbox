#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <FS.h>
#include "HTMLPAGE.h"

#define FORMAT_SPIFFS_IF_FAILED true
#define RECORD_SEP "\x1E"

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WebServer server(80);

const char* filename = "/posts.txt";

void sendPage(){
  Serial.println("GET /");
  server.send(200,"text/html",HTMLPAGE);  
}

void sendMessages(){
  Serial.println("GET /posts");
  File file = SPIFFS.open(filename, FILE_READ);
  if(!file){
      Serial.println("- failed to open file for reading");
  }

  server.streamFile(file,"text/plain");
  
  file.close();
}

void receiveMessage(){
  Serial.println("POST /post");
  int argCount = server.args();
  if (argCount == 0){
    Serial.println("zero args?");
  }
  
  File file = SPIFFS.open(filename, FILE_APPEND);
  if(!file){
      Serial.println("- failed to open file for writing");
  }
  if(argCount == 1){
    String line = server.arg(0);
    line.replace(String(RECORD_SEP),String(""));
    file.print(line);
    file.print(RECORD_SEP);
    Serial.print("New message: ");
    Serial.println(line);
  }
  file.close();
  
  server.send(200,"text.plain","");
}

void setup() {
  Serial.begin(115200);  
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  // initialize file (not totally sure this is necessary but YOLO)
  File file = SPIFFS.open(filename, FILE_READ);
  if(!file){
      file.close();
      File file_write = SPIFFS.open(filename, FILE_WRITE);
      if(!file_write){
          Serial.println("- failed to create file?!?");
      }
      else{
        file_write.print("");
        file_write.close();
      }
  }else{
    file.close();
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ChatterBox");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // init http server
  server.on("/", HTTP_GET, sendPage);
  server.on("/index.html", HTTP_GET, sendPage);
  server.on("/posts", HTTP_GET, sendMessages);
  server.on("/post", HTTP_POST, receiveMessage);
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();

  // type a "c" in the serial monitor to clean the database
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    if(incomingByte == 99){
      Serial.println("overwriting file...");
      File file_write = SPIFFS.open(filename, FILE_WRITE);
      file_write.print("");
      file_write.close();      
      Serial.println("file overwritten :)");
    }
  }
  
}
