#include <Arduino.h>
#include "propeller.h" 
#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"

#define hallSensorPin 1
const int TIMER_INTERVAL = 2000;


const float propeller_calibration = 0.193; // number of meters the propeller "advances" trough air during one complete rotation

int pulseCount = 0; // number of sixth of rotations
float windSpeed = 0; //Windspeed in Km/h

void hallSensorInt();

SAMDTimer ITimer0(TIMER_TC3);
SAMD_ISR_Timer ISR_Timer;


void TimerHandler(void)
{
  ISR_Timer.run();
}

void prop_setup() {
  pinMode(hallSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), hallSensorInt, FALLING);

  ITimer0.attachInterruptInterval(100 * 1000, TimerHandler); // Interval in microsecs
  ISR_Timer.setInterval(TIMER_INTERVAL, calculateWindSpeed);
}

// calculate the speed of the wind during a window of time.
// This function should be called at regular interval (e.g. every 2 seconds).
void calculateWindSpeed() {

  Serial.println(pulseCount);
  float rps = (1000 * pulseCount) / (2000 * 6.); // Rotations per seconds
  Serial.println(rps);
  windSpeed = rps * propeller_calibration * 3.6; // wind speed calculation in Km/h
  Serial.println(windSpeed);

  pulseCount = 0;
}

void hallSensorInt() {
  pulseCount++;
}

float getWindSpeed() {
  return windSpeed;
}

