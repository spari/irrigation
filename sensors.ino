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
   int adc_raw = 0;

   /*
      Get average of 5 readings.
      This is because occassionally the LM35 shows an [inverted] spike.
   */
   const int samples = 10;
   for (int i=0; i<samples; i++) {
      adc_raw += analogRead(gpio.temp_sensor);
      delay(50);
   }
   adc_raw /= samples;

   /* 
      LM35 temperature sensor is powered by 5V. 
      Has a linear accuracy of 10mV/C.

      ESP8266 uses 3.3v GPIO, and ADC has a range of 1024 bits.
      So resolution is 3300mv/1023 millivolts per bit.

      As per datasheet, the LM35 v_out = 250mV @ 25C.
      That is 10mV/C, so scale by 10.
   */
   float celsius;
   int AVG_COUNT = 100;

   /*
      Temperature inside the enclosure may be few degrees higher than outside.
      Adjust calibration accordingly to match outside temperature reading.
   */
   float calibration = 0.78;

   for (int i=0; i<AVG_COUNT; i++) {
      celsius += (adc_raw * 3300.0/1023) / 10;
      delay(5);
   }
   celsius /= AVG_COUNT;
   celsius *= calibration;

   log_debug("Temp=%f°C raw=%d", celsius, adc_raw);

   return (int)(celsius*10)/10.0;
}

