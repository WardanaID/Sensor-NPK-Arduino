
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial Serialsoft (D8, D7);
String data;
char c;
int i;
int j=0;
String dataIn;
String dt[10];
String myStatus = "";

unsigned long interval=10000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.


void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);
  Serialsoft.begin(9600); // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      digitalWrite(LED_BUILTIN, LOW);
      delay(5000);     
    } 
    Serial.println("\nConnected.");
    digitalWrite(LED_BUILTIN, HIGH);
  }
boolean parsing=false;
unsigned long currentMillis = millis(); // grab current time
while (Serialsoft.available()>0)
             {
           delay(10);
           char inChar = (char)Serialsoft.read();
           dataIn += inChar;
           Serial.println ("data masuk: ");
           Serial.println (dataIn);
           parsing = true; 
           digitalWrite(LED_BUILTIN, LOW);
            }
            if(parsing){
            dataIn="";
            Serial.println ("parsing");
            int j=0;
            dt[j]="";
              for(i=1;i<dataIn.length();i++)
              {
                     if ((dataIn[i] == '#') || (dataIn[i] == ','))
                     {
                           j++;
                          dt[j]=""; //inisialisasi variabel array dt[j]                     
                     }
                     else
                     {
                          dt[j] = dt[j] + dataIn[i];
                     }
              }
              Serial.print ("parsing selesai");
              Serial.println (dataIn);
              Serial.print("data 1 : ");
              Serial.println(dt[0].toInt());
              Serial.print("data 2 : ");
              Serial.println(dt[1].toInt());
              Serial.print("data 3 : ");
              Serial.println(dt[2].toInt());
              Serial.print("\n\n");
              parsing=false;
                ThingSpeak.setField(1, dt[0]);
                ThingSpeak.setField(2, dt[1]);
                ThingSpeak.setField(3, dt[2]);
                
                // set the status
                ThingSpeak.setStatus(myStatus);
                
                // write to the ThingSpeak channel
                int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
                if(x == 200){
                  Serial.println("Channel update successful.");
                }
                else{
                  Serial.println("Problem updating channel. HTTP error code " + String(x));
                }
                
                            } 


}
