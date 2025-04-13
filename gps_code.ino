// Code to get GPS coordinates on TTGO T-BEAM
// and show them on Serial Monitor

#include <HardwareSerial.h>
#include <TinyGPS++.h>

#define GPS_SERIAL_NUM 1
#define GPS_RX_PIN 3
#define GPS_TX_PIN 1

HardwareSerial GPSSerial(GPS_SERIAL_NUM);

TinyGPSPlus gps; // tinyGPSplus lib by Mikal Hart

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("Hello GPS");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    GPSSerial.write(c);
  }

  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);    
    if (gps.encode(GPSSerial.read())) {
      if (gps.location.isValid()) {
        Serial.print("Latitude/Longitude: ");
        Serial.print(gps.location.lat(), 6);
        Serial.print("/");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }
  delay(2000);
}