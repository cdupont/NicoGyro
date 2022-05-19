#include <Arduino.h>
#include <Wire.h>
#include "compass.h"
#include "bmm150.h"
#include "bmm150_defs.h"
 

BMM150 bmm = BMM150();
 

void compass_setup()
{

  Serial.begin(9600);
 
  if(bmm.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while(1);
  } else {
    Serial.println("Initialize done!");
  }

}
 

float getWindDirection()
{
  bmm150_mag_data value;
  bmm.read_mag_data();

  value.y = bmm.raw_mag_data.raw_datay;
  value.z = bmm.raw_mag_data.raw_dataz;

  float heading = atan2(value.z, value.y);

  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;

  float headingDegrees = heading * 180/M_PI;
  return(headingDegrees);

}
