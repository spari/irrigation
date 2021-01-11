#include <ConfigManager.h>

const char *settingsHTML = (char *)"/settings.html";
const char *stylesCSS = (char *)"/styles.css";
const char *mainJS = (char *)"/main.js";

#include "config_manager.h"

struct Config config;

struct Metadata {
   int8_t version;
} meta;

const char* APName = "{ap_name}"; //scrubbed
const char* APPassword = "{ap_password}"; //scrubbed
const char* APIUsername = "{api_username}"; //scrubbed
const char* APIPassword = "{api_password}"; //scrubbed

void APCallback(WebServer *server) {
   server->on("/styles.css", HTTPMethod::HTTP_GET, [server](){
      configManager.streamFile(stylesCSS, mimeCSS);
   });

   DebugPrintln(F("AP Mode Enabled. You can call other functions that should run after a mode is enabled ... "));
}

void APICallback(WebServer *server) {
   server->on("/disconnect", HTTPMethod::HTTP_GET, [server](){
      configManager.clearWifiSettings(false);
   });

   server->on("/settings.html", HTTPMethod::HTTP_GET, [server](){
      if (!server->authenticate(APIUsername, APIPassword)) {
         return server->requestAuthentication();
      } 
      configManager.streamFile(settingsHTML, mimeHTML);
   });

   server->on("/styles.css", HTTPMethod::HTTP_GET, [server](){
      configManager.streamFile(stylesCSS, mimeCSS);
   });

   server->on("/main.js", HTTPMethod::HTTP_GET, [server](){
      configManager.streamFile(mainJS, mimeJS);
   });
}

/*
   If this callback has been called it means a new configuration has
   been saved. Sets the config.configured flag to true and restarts the 
   board. See comments in setup().
*/
void SavedCallback()
{
   config.configured = true;
   configManager.save();

   Serial.println("Configuration Saved. Restarting Device...");
   delay(1000);
   ESP.restart();
}

void init_config_manager()
{
   Serial.println("Intializing WiFi and ConfigManager.");

   meta.version = 3;

   // Setup config manager
   configManager.setWifiConnectRetries(100);
   configManager.setWifiConnectInterval(500);
   configManager.setAPName(APName);
   configManager.setAPPassword(APPassword);
   configManager.setAPFilename("/index.html");

   /*
      Settings variables
   */
   // Controller
   AddField(controller.watering_hours);
   AddField(controller.min_duration);
   AddField(controller.max_duration);
   AddField(controller.min_temp);
   AddField(controller.max_temp);

   // Time
   AddFieldStr(time.timezone);
   AddField(time.daylight_savings);
   AddFieldStr(time.ntp_server);

   // MQTT
   AddFieldStr(mqtt.server);
   AddField(mqtt.port);
   AddFieldStr(mqtt.username);
   AddFieldStr(mqtt.password);
   AddFieldStr(mqtt.client_id);
   AddFieldStr(mqtt.base_topic);
   AddFieldStr(mqtt.log_topic);

   // General
   AddField(configured);

   // Meta Settings
   configManager.addParameter("version", &meta.version, get);

   // Init Callbacks
   configManager.setAPCallback(APCallback);
   configManager.setAPICallback(APICallback);
   configManager.setSavedCallback(SavedCallback);

   configManager.begin(config);
}

