#include <EasyButton.h>

#include "local.h"

EasyButton ApModeButton(ApSwitch::gpio.ap_button);

/*
   C wrappers to C++ methods for passing to callbacks.
*/
void _ap_button_press() { ap_switch.ap_button_press(); }
void _ap_button_isr() { ap_switch.ap_button_isr(); }

ApSwitch::ApSwitch() {}

void ApSwitch::ap_button_press()
{
   Serial.println("ap_button_press");
   ap_mode= true;
}

void ApSwitch::ap_button_isr()
{
   Serial.println("ap_button_isr");
   ApModeButton.read(); 
}

void ApSwitch::init()
{
   extern ConfigManager configManager;

   ApModeButton.begin();
   ApModeButton.onPressed(_ap_button_press);
   ApModeButton.enableInterrupt(_ap_button_isr);

   Serial.println("Waiting for AP Mode request...");
   for (int i=0; i<ap_mode_retries; i++) {
      if (ap_mode) {
         Serial.println("AP Mode Request Received.");
         configManager.setWifiMode(ap);
         break;
      }
      delay(1000);
   }
}

