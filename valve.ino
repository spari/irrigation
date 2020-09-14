#include "valve.h"

#include <Arduino.h>

#include "local.h"

const int ValveOn = HIGH;
const int ValveOff = LOW;

Valve::Valve() {}

void Valve::init()
{
   pinMode(gpio.valve, OUTPUT);

   digitalWrite(gpio.valve, ValveOff);
}

void Valve::open()
{
   log_info("  --> Opening Valve.");
   digitalWrite(gpio.valve, ValveOn);
   valve_opened = true;
}

void Valve::close()
{
   log_info("  --> Closing Valve.");
   digitalWrite(gpio.valve, ValveOff);
   valve_opened = false;
}

/*
   Ideally use a sensor to confirm that the valve has been turned on. 
   For example:
      - A water moisture sensor at outlet (ideal).
      - A flow detector to detect flow in pipe (less ideal).
   Essential if this is to be unmanned for extended time period.
   Or to detect human errors, such as if someone shuts off the main valve;
   the irrigation controller will continue to function giving the illusion
   that irrigation is taking place.
*/
bool Valve::is_open()
{
   return valve_opened;
}


