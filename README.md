# Irrigation Controller 
Details at: https://matrix108.wordpress.com/2019/08/10/irrigation-controller-v2/

## History
| Date       | Version | Description                                                  |
| ---------- | ------- | ------------------------------------------------------------ |
| 2019-08-10 | 1.0     | - First working version.                                     |
| 2020-07-18 | 2.0     | - Uses ConfigManager for managing WiFi and application configuration. <br />- No longer uses TimeAlarms for keeping alive or checking mqtt, ntp, and ota. Removed critical section around valve open/close as it will not be interrupted by timers. |

## Dependencies

### Project Library
- [iot_utils](https://github.com/spari/iot_utils)

### Third-Party Libraries
- ConfigManager

## Uploading Instructions
### Upload Sketch Data
Upload the sketch's data directory to the SIFFS via:
    Arduino IDE --> Tools --> ESP8266 Sketch Data Upload

### Upload Sketch
Before uploading the sketch set the following in the Arduino IDE:
* Arduino IDE --> Tools --> Board --> NodeMCU 1.0 (ESP-12E Module)
* Arduino IDE --> Tools --> Flash Size --> 4MB (FS:2MB  OTA:1MB)
* Arduino IDE --> Tools --> Erase Flash --> Only Sketch
* Arduino IDE --> Tools --> Port --> Set accordingly

