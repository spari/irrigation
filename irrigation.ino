#include "irrigation.h"

#include <Time.h>
#include <ConfigManager.h>

#include "local.h"

extern struct Config config;
extern ConfigManager configManager;

constexpr struct {
   uint8_t ConfigWait=1;
   uint8_t WiFiRetry=2;
   uint8_t NTPRetry=3;
   uint8_t MQTTRetry=4;
} StatusBlink;

void wifi_retry_callback()
{
   signaller.alert(StatusBlink.WiFiRetry);
}

void ntp_retry_callback()
{
   signaller.alert(StatusBlink.NTPRetry);
}

void mqtt_retry_callback()
{
   signaller.alert(StatusBlink.MQTTRetry);
}

void setup()
{
   Serial.begin(115200);
   delay(100);
   Serial.println();
   Serial.println("Initializing...");

   signaller.init();

   ap_switch.init();

   init_config_manager();

   ota_utils.init();

   /*
      If settings are not configured or still in AP mode, wait for settings to
      be configured.
      ConfigManager does a restart of device after configuration is saved
      (initiated from browser), bringing us back to this step after the restart.
   */
   if (!config.configured || configManager.getMode() == ap) {
      Serial.println("-- Not Configured --");
      return;
   }

   time_utils.init(&config.time, ntp_retry_callback);
   mqtt_utils.init(&config.mqtt, mqtt_retry_callback);
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
   if (config.configured && configManager.getMode() != ap) {
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
      signaller.alert(StatusBlink.ConfigWait);
   }

   configManager.loop();
}

