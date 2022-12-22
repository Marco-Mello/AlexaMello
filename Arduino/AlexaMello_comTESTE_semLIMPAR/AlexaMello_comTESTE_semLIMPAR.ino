/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  Code compatible with ESP8266 Boards Version 3.0.0 or above
  (see in Tools > Boards > Boards Manager > ESP8266)
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>



#include <Adafruit_NeoPixel.h>




//PROTOTIPOS


//const char* ssid = "Corsi";
//const char* password = "corsiEmarco";

const char* ssid = "Mello";
const char* password = "xicoxcomx";


String serverName = "http://192.168.249.211:1880";

unsigned long lastTime = 0;
unsigned long timerDelay = 10;




#define LED_PIN 3

#define LED_COUNT 826

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  String payload = "ON_20_#FFFFFF";
  while(1) testeProtocol(payload);

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


  //  if (WiFi.status() == WL_CONNECTED) {
  //    WiFiClient client;
  //    HTTPClient http;
  //  //contName++;
  //    String serverPath = "http://192.168.50.182:1880/teste";
  //
  //    // Your Domain name with URL path or IP address with path
  //    http.begin(client, serverPath.c_str());
  //
  //    int httpResponseCode = http.GET();
  //  }

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  //strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  //strip.setBrightness(102); // Set BRIGHTNESS to about 1/5 (max = 255)

}

void loop() {
  // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {





    getLEDQuartoStatus();

  }
}



void getLEDQuartoStatus()
{
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    //contName++;
    String serverPath = serverName + "/LEDQuartoStatus";

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();

      //teste de protocolo
     // payload = "OFF";


      if (payload == "ON")
      {
        Serial.println("Ligou");
        for (int i = 0; i < strip.numPixels(); i++)
        {
          if (i % 2) strip.setPixelColor(i, 255,   255,   255);
          else strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
      }
      else if (payload == "OFF")
      {
        Serial.println("Desligou");
        for (int i = 0; i < strip.numPixels(); i++)
        {
          strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
      }


      //   if ledQuartoStatus == 1


      // else if eldQUartosStatus ==0




    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  lastTime = millis();
}

void testeProtocol(String payload)
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
Serial.println(parametro[0]);
Serial.println(parametro[1]); 
//Serial.println(parametro[2]); 

 int estado;
 int brilho;
 int cor[3];

 brilho = parametro[1].toInt();
 
parametro[2].remove(0, 1);
Serial.println(parametro[2]); 
char char_array_TWO[parametro[2].length() + 1];
parametro[2].toCharArray(char_array_TWO, parametro[2].length() + 1);
int ret = hexString2int(char_array_TWO);
cor[0]= (ret & 0xFF0000) >> 16;
cor[1]= (ret & 0x00FF00) >> 8;
cor[2]= (ret & 0x0000FF);
//Serial.print("  >  ");
//Serial.println(ret,HEX);



 if(parametro[0] == "OFF"){ cor[0] = 0; cor[1] = 0; cor[2] = 0; }


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

//delay(500);

      strip.setBrightness(map(brilho,0,100,0,255));
      for (int i = 0; i < strip.numPixels(); i++)
        {
          if (i % 2) strip.setPixelColor(i, cor[0],   cor[1],   cor[2]);
          else strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
 
     /* if (payload == "ON")
      {
        //Serial.println("Ligou");
        for (int i = 0; i < strip.numPixels(); i++)
        {
          if (i % 2) strip.setPixelColor(i, 255,   255,   255);
          else strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
      }
      else if (payload == "OFF")
      {
        //Serial.println("Desligou");
        for (int i = 0; i < strip.numPixels(); i++)
        {
          strip.setPixelColor(i, 0,   0,   0);
          delayMicroseconds(1);
        }
        strip.show();
      }*/


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
