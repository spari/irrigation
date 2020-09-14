#ifndef local_h 
#define local_h 

/* 
 * Redirect log_[debug|info|warn|error] output to MQTT if enabled.
 * Else redirects to serial port.
 * Explicit calls like log_mqtt() and log_serial() are unaffected. 
 */
#define LOG_MQTT

#ifdef LOG_MQTT
#define log_proxy(fmt, ...) mqtt_utils.log_mqtt(fmt, ##__VA_ARGS__)
#else
#define log_proxy(fmt, ...) log_serial(fmt"\r\n", ##__VA_ARGS__)
#endif

#endif


