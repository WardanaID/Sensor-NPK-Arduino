//board uno

/*
tambah soil moisture di pin analog
integrasi dengan ESP8266
integrasi dengan ThingSpeak serta pustaka terkait di esp
--
tambah sensor pH tanah

*/




//pH tanah
#define DMSpin  6    // pin output untuk DMS
#define indikator  2   // pin output untuk indikator pembacaan sensor
#define adcPin 7      // pin input sensor pH tanah
//variabel     
float lastReading;
float pH;


#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
LiquidCrystal_I2C lcd(0x27, 20, 4);
void getText();
void updateLCD();

#define RE 12
#define DE 13

#define DHTPIN 8  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define soil1 A0
#define soil2 A1
#define soil3 A2
#define soil4 A3
 
//const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};
const byte phos[] = {0x01,0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C};
const byte nitro[] = {0x01,0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0};
 
int lcdstamp = 1;
int countr = 1;

byte values[11];
SoftwareSerial mod(10,11);

SoftwareSerial Serialsoft (2, 3);
String data;
char c;
 
unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.
  
void setup() {
  dht.begin();
  Serial.begin(9600);
  mod.begin(9600);
  Serialsoft.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  lcd.begin();
  lcd.backlight();
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("NPK Sensor Wardana");
  lcd.setCursor(0, 3);
  lcd.print("Memuat...");
  pinMode(DMSpin, OUTPUT);
  pinMode(indikator, OUTPUT);
  digitalWrite(DMSpin,HIGH);     // non-aktifkan DMS

}
 
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int readsoil1 = analogRead(soil1);
  int readsoil2 = analogRead(soil2);
  int readsoil3 = analogRead(soil3);
  int readsoil4 = analogRead(soil4);

  int persensoil1 = map(readsoil1, 1023, 0, 0, 100);
  int persensoil2 = map(readsoil2, 1023, 0, 0, 100);
  int persensoil3 = map(readsoil3, 1023, 0, 0, 100);
  int persensoil4 = map(readsoil4, 1023, 0, 0, 100);

  Serial.print("Soil 1 : ");
  Serial.print(persensoil1);
  Serial.print("%");

  Serial.print("Soil 2 : ");
  Serial.print(persensoil2);
  Serial.print("%");

  Serial.print("Soil 3 : ");
  Serial.print(persensoil3);
  Serial.print("%");

  Serial.print("Soil 4 : ");
  Serial.print(persensoil4);
  Serial.print("%");



  byte val1,val2,val3;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);
  
  lcd.clear();

  if (countr >= 3) {
    lcdstamp++;
    countr = 1;
  }

  if (lcdstamp == 5) {
    lcdstamp = 1;
  }



  switch (lcdstamp){
    case 1:
      lcd.setCursor(0, 0);
      if (val3 >= "255"){
        lcd.print("NPK tdk. Terhubung")
      }
      lcd.print("Kandungan tanah");
      lcd.setCursor(0, 1);
      lcd.print("Nitro.  : ");
        lcd.print(val1);
      lcd.print("mg/kg");

      lcd.setCursor(0, 2);
      lcd.print("Phosp.  : ");
        lcd.print(val2);
      lcd.print("mg/kg");

      lcd.setCursor(0, 3);
      lcd.print("Pottas. : ");
        lcd.print(val3);
      lcd.print("mg/kg");
      lcd.setCursor(0,)
      delay(2000);
      countr++;
      break;

    case 2:
        lcd.setCursor(0, 0);
        lcd.print("Kelembaban tanah");
        lcd.setCursor(0, 1);
        lcd.print("Soil1:");
        lcd.print(persensoil1);
        lcd.print("% ");

        lcd.setCursor(0, 2);
        lcd.print("Soil2:");
        lcd.print(persensoil2);
        lcd.print("% ");

        lcd.setCursor(0, 3);
        lcd.print("Soil3:");
        lcd.print(persensoil3);
        lcd.print("% ");

        lcd.setCursor(10, 1);
        lcd.print("Soil4:");
        lcd.print(persensoil4);
        lcd.print("% ");
        delay(2000);
        countr++;
        break;

    case 3:
        lcd.setCursor(0, 0);
        lcd.print("Udara");
        lcd.setCursor(0, 1);
        lcd.print("Suhu      : ");
        lcd.print(t);
        lcd.print("`C ");

        lcd.setCursor(0, 2);
        lcd.print("Kelembapan: ");
        lcd.print(h);
        lcd.print("% ");
        delay(2000);
        countr++;
        break;

    case 4:
    
        digitalWrite(DMSpin,LOW);      // aktifkan DMS
        digitalWrite(indikator, HIGH); // led indikator built-in ESP32 menyala
          pH = (-0.0233 * analogRead(adcPin)) + 12.698;  // ini adalah rumus regresi linier yang wajib anda ganti!
          if (pH != lastReading) { 
          lastReading = pH; 
          }
        Serial.print("ADC=");
        Serial.print(analogRead(adcPin));             
        Serial.print(" pH=");
        Serial.println(lastReading,1); // menampilkan nilai pH di serial monitor 
        digitalWrite(DMSpin,HIGH);
        digitalWrite(indikator,LOW);
        lcd.setCursor(0, 0);
        lcd.print("pH Tanah:");
        lcd.setCursor(0, 1);
        lcd.print("pH Tanah: ");
        lcd.print(lastReading,1);

        lcd.setCursor(0, 2);
        lcd.print("ADC: ");
        lcd.print(analogRead(adcPin));
        delay(2000);
        countr++;
        break;
    }


  Serial.println("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");
  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");

unsigned long currentMillis = millis(); // grab current time
  while (Serialsoft.available()>0){
    delay(10);
    c = Serialsoft.read();
    data+=c;
    }
  
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    Serialsoft.print("*" + (String(val1)) + "," + (String(val2)) + ","+ (String(val3)) + "," + (String(persensoil1)) + "," + (String((persensoil2)) + "," + (String(persensoil3)) + "," + (String(persensoil4)) + "#"));
    //Serial.print("*" + val1 + "," + val2 + ","+ val3 + "," + persensoil1 + "," + persensoil2 + "," + persensoil3 + "," + persensoil4 + "#");
   previousMillis = millis();

}
}
 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(mod.read(),HEX);
    values[i] = mod.read();
    Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}