#ifndef controller_mqtt_h
#define controller_mqtt_h

/*
 * MQTT Topics 
 */
const char* TOPIC_IRRIGATE      = "/irrigate"; 
const char* TOPIC_TEMPERATURE   = "/temp";
const char* TOPIC_DURATION      = "/duration";
const char* TOPIC_RESULT        = "/result";
const char* TOPIC_RESTART       = "/restart";
const char* TOPIC_SYSINFO       = "/sysinfo";
const char* TOPIC_HELLO         = "/hello";

void print_sysinfo();

#endif

