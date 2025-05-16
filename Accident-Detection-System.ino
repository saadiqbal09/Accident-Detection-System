#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
SoftwareSerial SIM800(7, 8);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  Serial.begin(19200);
  ss.begin(GPSBaud);
  SIM800.begin(19200);
  Serial.println("SIM800 ready...");

  SIM800.print("AT+CMGF=1\r");
  delay(100);
  SIM800.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.println(ax);
  delay(10);

  if (ax > 5000 || ax < -5000) {
    sendsms();
    digitalWrite(13, HIGH);
    delay(15000);
    if (digitalRead(12) == LOW) {
      digitalWrite(13, LOW);
    } else {
      digitalWrite(13, HIGH);
    }
  }
}

void sendsms() {
  if (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      Serial.println("GPS FIXED");
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
    }
  }

  SIM800.println("AT+CMGS=\"+917499044978\"");
  delay(100);
  Serial.println("Sending SMS");
  String message = "Accident Alert!!!\n"
                   "A CRASH of vehicle has been detected, please send immediate assistance.\n"
                   "Vehicle: (MH 27 BJ 1094)\n"
                   "Location: Lat: " + String(gps.location.lat(), 6) + ", Long: " + String(gps.location.lng(), 6) + "\n"
                   "https://www.google.com/maps/?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  Serial.println(message);
  SIM800.println(message);
  delay(100);
}
