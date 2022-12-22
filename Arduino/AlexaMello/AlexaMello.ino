/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  Code compatible with ESP8266 Boards Version 3.0.0 or above
  (see in Tools > Boards > Boards Manager > ESP8266)
*/


//#define DEBUG

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>



#include <Adafruit_NeoPixel.h>




//PROTOTIPOS


const char* ssid = "CLARO_2GBC8514";
const char* password = "E7BC8514";


String serverName = "http://192.168.0.60:1880";

unsigned long lastTime = 0;
unsigned long timerDelay = 10;




#define LED_PIN 3

#define LED_COUNT 826

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
 
}

void loop() {

 //SOLICITA A REQUISICAO APOS TIMERDELAY definido 10ms
  if ((millis() - lastTime) > timerDelay) {

    getLEDQuartoStatus();

  }
}



void getLEDQuartoStatus()
{
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String serverPath = serverName + "/LEDQuartoStatus";

    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0){
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      protocoloACIONAfita(payload);
    }
    else{
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  lastTime = millis();
}

void protocoloACIONAfita(String payload)
{





char char_array[payload.length() + 1];
payload.toCharArray(char_array, payload.length() + 1);

char *testes = NULL;
String parametro[3];

 testes = strtok(char_array, "_");
    int i = 0;
    while(testes != NULL)
    {
        //Serial.println(testes);
        parametro[i] = testes;
        i=i+1;
        testes = strtok(NULL, "_"); 
        
    }


 int estado;
 int brilho;
 int cor[3];

 brilho = parametro[1].toInt();
 
parametro[2].remove(0, 1);
 
char char_array_TWO[parametro[2].length() + 1];
parametro[2].toCharArray(char_array_TWO, parametro[2].length() + 1);
int ret = hexString2int(char_array_TWO);
cor[0]= (ret & 0xFF0000) >> 16;
cor[1]= (ret & 0x00FF00) >> 8;
cor[2]= (ret & 0x0000FF);

 if(parametro[0] == "OFF"){ cor[0] = 0; cor[1] = 0; cor[2] = 0; }


#ifdef DEBUG
Serial.println(parametro[0]);
Serial.println(parametro[1]); 
  Serial.println(parametro[2]);

 Serial.print("Brilho: ");
 Serial.print(brilho);
 Serial.print("%");

 Serial.print("Cor > ");
 Serial.print("R: ");
 Serial.print(cor[0]);
 Serial.print(" G: ");
 Serial.print(cor[1]);
 Serial.print(" B: ");
 Serial.println(cor[2]);
 Serial.println();
 Serial.println();
 Serial.println();
#endif

//delay(500);

      strip.setBrightness(map(brilho,0,100,0,255));
      for (int i = 0; i < strip.numPixels(); i++)
        {
          if (i % 2) strip.setPixelColor(i, cor[0],   cor[1],   cor[2]);
          else strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
}


int hexString2int(char *s) 
{
  int x = 0;
  for(;;) {
    char c = *s;
    if (c >= '0' && c <= '9') {
      x *= 16;
      x += c - '0'; 
    }
    else if (c >= 'A' && c <= 'F') {
      x *= 16;
      x += (c - 'A') + 10; 
    }
    else break;
    s++;
  }
  return x;
}
