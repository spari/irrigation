#include "sensors.h"

#include <Arduino.h>

#include "local.h"

Sensors::Sensors() {}

void Sensors::init()
{
   pinMode(gpio.temp_sensor, INPUT);
}

float Sensors::get_temp()
{
   int val=0;

   /*
      Get average of 5 readings.
      This is because occassionally the LM35 shows an [inverted] spike.
   */
   for (int i=0; i<5; i++) {
      val += analogRead(gpio.temp_sensor);
      delay(100);
   }
   val /= 5;

   /* 
      LM35 powered by 5V.
      Temperature was off by a significant amount.
      Had to scale it down by an error correction factor. 
   */
   float error_correction = 0.057;
   float celsius = val * 5000/1024 * error_correction;

   log_debug("Temp=%f°C raw=%d", celsius, val);

   return (int)(celsius*10)/10.0;
}

