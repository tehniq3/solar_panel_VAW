// 
// rx - ver.0
// add wifimanager

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif
//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

//#include <ESP8266WiFi.h>
//--------------Your Data----------------//
//String apiKey = "API_keyL"; // <<<<< YOUR API KEY
String apiKey = "9K0DL8Q37OORS12R"; // <<<<< YOUR API KEY
//const char* ssid = "your_SSID"; // <<<<<<<< Your Wi-Fi SSID
//const char* password = "password_SSID"; // <<<<<<<< Your Wi-Fi Pasword

//--------------------------------------//
const char* server = "api.thingspeak.com";

int a0, v0;
float volts;
float amps;
float power;

WiFiClient client;
unsigned char buff[10], i;
String buffer1, buffer2;

void setup()
{
  Serial.begin(9600);
  delay(10);
  //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
/*
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Se conecteaza la ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectat");
  */

  
}
void loop()
{
  if (Serial.available() > 0)
  {
    delay(100);
    while (Serial.available() > 0)
    {
      buffer1 = Serial.readString();
      if (buffer1[0] == '*')
      {
        if (buffer1[9] == '#')
        {
          Serial.println(buffer1);
          v0 = ((buffer1[1] - 0x30) * 1000 + (buffer1[2] - 0x30) * 100 + (buffer1[3] - 0x30) * 10 + (buffer1[4] - 0x30));
          a0 = ((buffer1[5] - 0x30) * 1000 + (buffer1[6] - 0x30) * 100 + (buffer1[7] - 0x30) * 10 + (buffer1[8] - 0x30));
        }
      }
    }

    volts = v0 / 100.;
    amps = a0 / 100.;
    power = volts * amps;

    Serial.println(" ----------------------- ");
    Serial.print(v0);
    Serial.print(" + ");
    Serial.println(a0);
    Serial.print(volts);
    Serial.print("V, ");
    Serial.print(amps);
    Serial.println("A ----------------------- ");

    if (client.connect(server, 80))
    {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(volts);
      postStr += "&field2=";
      postStr += String(amps);
      postStr += "&field3=";
      postStr += String(power);
      postStr += "\r\n\r\n";
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
      Serial.println(postStr);
    }
    client.stop();
    Serial.println("I'm waitting new data...");

    // delay(20000);
  }
}
//-------------Electronics-project-hub>com-------------//
