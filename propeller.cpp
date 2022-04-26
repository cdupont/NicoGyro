#include <Arduino.h>
#include "propeller.h" 

#define hallSensorPin 1


const float propeller_calibration = 0.193; // number of meters the propeller "advances" trough air during one complete rotation

int rotationCount = 0; // number of sixth of rotations
float previousTopTime = 0;  

float windSpeed = 0; //Windspeed in Km/h

void hallSensorInt();

void prop_setup() {
  float previousTopTime = millis();
  pinMode(hallSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), hallSensorInt, FALLING);
}

void calculateWindSpeed() {
   
  float now = millis();
  float durationRot = now - previousTopTime; //TODO: overflow?
  previousTopTime = now;
  
  float rps = (1000/6) * rotationCount / durationRot; // Rotations per seconds
  windSpeed = rps * propeller_calibration * 3.6; // wind speed calculation in Km/h

  rotationCount = 0;
}

void hallSensorInt() {
  rotationCount++;
}

float getWindSpeed() {
  return windSpeed;
}

