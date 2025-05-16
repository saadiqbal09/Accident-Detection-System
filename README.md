# Accident Detection and Alert System

## Overview
An Arduino-based system to detect vehicle accidents and send SMS alerts with location data using MPU6050, GPS, and SIM800 GSM modules.

## Features
- Crash detection using MPU6050 accelerometer.
- GPS location tracking with TinyGPS++ library.
- SMS alerts with Google Maps link via SIM800.

## Hardware
- Arduino board
- MPU6050 accelerometer/gyroscope
- GPS module (e.g., NEO-6M)
- SIM800 GSM module

## Software
- Arduino IDE
- Libraries: TinyGPS++, SoftwareSerial, I2Cdev, MPU6050, Wire

## Setup
1. Connect the MPU6050, GPS, and SIM800 modules to the Arduino.
2. Install the required libraries in the Arduino IDE.
3. Upload the code (`Accident_Detection_System.ino`) to the Arduino.
