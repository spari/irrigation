# Irrigation Controller 
Details at: https://matrix108.wordpress.com/2019/08/10/irrigation-controller-v2/

## History
| Date       | Version | Description                                                  |
| ---------- | ------- | ------------------------------------------------------------ |
| 2019-08-10 | 1.0     | - First working version.                                     |
| 2021-01-11 | 2.0     | - Uses ConfigManager for managing WiFi and application configuration. <br />- No longer uses TimeAlarms for keeping alive or checking mqtt, ntp, and ota. Removed critical section around valve open/close as it will not be interrupted by timers. |
| 2022-02-22 | 2.1     | - No longer defaults to AP mode when WiFi connection times out, instead tries indefinitely. Useful when device is out in the field with unreliable network connection or power outages. <br />- Added callbacks for retry of WiFi, NTP, and MQTT. <br />- Using simpler signaller to notify device status (see Diagnostics). |

## Dependencies

### Project Library
- [iot_utils](https://github.com/spari/iot_utils)

### Third-Party Libraries
1. ConfigManager - I've forked ConfigManager with the following changes:
   1. Added a function that sets a callback that is called when new settings are saved, so that we can take appropriate action (such as restart device with the new settings).
   2. Added a setWifiMode() to set the WiFi mode at startup. The default was to downgrade to AP mode when it cannot establish a WiFi connection. This is problematic if device is out in the field with unreliable network connection or power outages. Instead we indefinitely try for WiFi connection and set AP mode only if a button is pressed during startup.
   3. Added a WifiRetryCallback() to inform each time a WiFi connection attempt is made. This can be used to signal the user that it is trying to connect to Wifi. See Diagnostics table below.
2. TimeAlarms.

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

## Setup

### Configure WiFi Settings

First supply your WiFi credentials to the device:

1. Put device in AP (Access Point) mode, by  pressing the AP button within 20 seconds of the device being restarted.
2. The Red LED will flash 1 blink per second indicating it is AP mode.
3. Connect to "irrigation" SSID.
4. Open a browser and put in http://192.168.123.1.
5. Enter your router's SSID and password and save. Device will automatically restart.
6. The Status LED will flash 1 blink per second indicating you need to next configure the device. See next section, Device Settings.

### Configure Device Settings

Next setup the device by configuring the device settings:

1. Find the IP assigned to your device (see your attached devices on your router or use nmap to scan for connected devices (eg: *nmap -sP 192.168.1.0/24*)).
2. Open a browser and type in http://{device_ip}/settings.html.
3. Change any settings as required and save. Device will automatically restart.
4. Wait 25 seconds (the device waits first 20 seconds to give you an opportunity to put it in AP mode, then proceeds to connect to WiFi, NTP, MQTT, and starts the controller).
5. The Heartbeat LED will flash 1 blink per second indicating device is up and running.

## Diagnostics

The state of the device is conveyed by the number of times an LED flashes. Useful if device is located remotely, a technician can quickly determine what is at fault.

Blink column in table below are the number of rapid blinks per 1-2 seconds (with roughly 1-2 second pause between each set of blinks, depending on the connection timeout delay for each operation).

In my case, the Status LED is Red, and Heartbeat LED is Green.

| LED   | Number of Blinks | Description                             | Comment/Diagnostics                                          |
| ----- | ---------------- | --------------------------------------- | ------------------------------------------------------------ |
| Both  | Off              | First 20 seconds.                       | 1. Press AP button to put in AP mode if required, within this 20 seconds.<br />2. Else wait, it will continue with normal startup. |
| Green | 1                | Normal.                                 | Heartbeat LED. Indicates main loop is functioning.           |
| Red   | 1                | Waiting for device to be configured.    | 1. See "Configure Device Settings".                          |
| Red   | 2                | Attempting to connect to WiFi.          | 1. Check if WiFi router is up.<br />2. Check if WiFi signal strength at device.<br />3. Check WiFi SSID and password. <br />4. You might not have configured WiFi settings correctly. See "Configure WiFi Settings". |
| Red   | 3                | Attempting to get time from NTP.        | 1. Check if WiFi router is up.<br />2. Check Internet connection.<br />3. Check if NTP server is up/reachable. |
| Red   | 4                | Attempting to connect with MQTT server. | 1. Check if WiFi router is up.<br />2. Check Internet connection. <br />3. Check if MQTT server is up. |

