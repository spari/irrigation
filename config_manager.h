#ifndef config_manager_h
#define config_manager_h

#include <ConfigManager.h>

#include <time_utils.h>
#include <mqtt_utils.h>

#include "valve.h"
#include "controller.h"

/*
 * See each struct definition for respective default values.
 * These get overriden by configuration entered via web 
 * interface ./data/settings.html via ConfigManager.
 */
struct Config {
   struct time_conf time;
   struct mqtt_conf mqtt;
   struct controller_conf controller;

   /*
    * Note: ConfigManager assigns a default value of -1 for int types, 
    * overriding default value of 0. 
    * 
    * This happens when ConfigManager initializes the config params from 
    * SPIFFS when it has not been stored before. So it can have value 
    * of 255 (0xFF), which translates to -1 for int type, and gets 
    * gets wrongly coerced to true for boolean types.
    *
    * To prevent such coercing and consequent wrong evaluation, do an 
    * an explicit comparison: "if (configured == true)" or 
    * "if (configured != true)", and keep 'configured' as an int type, so 
    * that this is an int comparision, not a boolean comparison. 
    */
   int configured = false;
};

ConfigManager configManager;

#define AddField(field) configManager.addParameter(#field, &config.field)
#define AddFieldStr(field) configManager.addParameter(#field, config.field, sizeof(config.field))

void init_config_manager();

#endif
