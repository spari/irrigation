#include "irrigation.h"

#include <TimeAlarms.h>

#include "local.h"

#define LOCAL_MQTT

struct Config config;
int Timer1SecId;

/*
   Called every second. Keep-alive/reconnect functions are put here. 
*/
void timer1sec_callback()
{
   time_utils.reconnect(10);
   mqtt_utils.reconnect();
   mqtt_utils.loop();
}

/*
   Currently configuration is stored right in code and uses git smudge 
   and clean process to scrub sensitive configuration from code. 
   TODO: Store and read this from a config file stored in SPIFFS.
*/
void init_config()
{
   strcpy(config.wifi.ssid, "{wifi_ssid}"); //scrubbed
   strcpy(config.wifi.password, "{wifi_password}"); //scrubbed

#ifndef LOCAL_MQTT
   strcpy(config.mqtt.server, "{mqtt_server}"); //scrubbed
   config.mqtt.port = atoi("{mqtt_port}"); //scrubbed
   strcpy(config.mqtt.username, "{mqtt_username}"); //scrubbed
   strcpy(config.mqtt.password, "{mqtt_password}"); //scrubbed
#endif

   strcpy(config.mqtt.client_id, "irrigation-mcu0");
   strcpy(config.mqtt.base_topic, "/home-iot/irrigation-mcu0");
}

void setup()
{
   Serial.begin(115200);

   init_config();

   signaller.init();
   wifi_utils.init(&config.wifi);
   ota_utils.init();
   time_utils.init(&config.time);
   mqtt_utils.init(&config.mqtt);
   sensors.init();
   valve.init();
   controller.init(&config.controller);

   print_sysinfo();
   log_info("System Ready.");

   Timer1SecId = Alarm.timerRepeat(1, timer1sec_callback);
}

boolean check_hour(uint8_t hour)
{
   return (config.controller.watering_hours >> (23-hour)) & 0x01;
}

void loop()
{
   ota_utils.loop();

   time_t ctime = now();

   // Alarm.delay() must be called at least for TimeAlarms to be enabled. 
   Alarm.delay(1);

   if (check_hour(hour(ctime)) && minute(ctime) == 0 && second(ctime) < 30) {
      controller.irrigate();               
   }
   signaller.ok();
}

