#include "irrigation.h"

#include <Time.h>
#include <ConfigManager.h>

#include "local.h"

extern struct Config config;

void setup()
{
   Serial.begin(115200);

   init_config_manager();

   signaller.init();
   ota_utils.init();

   /*
      If not configured, wait for system to be configured.
      Else proceed with the normal setup.
   */
   if (config.configured != true) {
      Serial.println("-- Not Configured --");
      return;
   }

   time_utils.init(&config.time);
   mqtt_utils.init(&config.mqtt);
   sensors.init();
   valve.init();
   controller.init(&config.controller);

   print_sysinfo();
   log_info("System Ready.");
}

boolean check_hour(uint8_t hour)
{
   return (config.controller.watering_hours >> (23-hour)) & 0x01;
}

void loop()
{
   extern ConfigManager configManager;

   configManager.loop();

   if (config.configured == true) {
      time_utils.reconnect(10);
      mqtt_utils.reconnect();
      mqtt_utils.loop();
      ota_utils.loop();

      time_t ctime = now();

      if (check_hour(hour(ctime)) && minute(ctime) == 0 && second(ctime) < 30) {
         controller.irrigate();
      }

      signaller.ok();
   }
   else {
      /*
         Wait for system to be configured.
      */
      signaller.warn(0b1000);
   }
}

