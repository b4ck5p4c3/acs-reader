#ifndef MCU_CONFIG_H_
#define MCU_CONFIG_H_

#include "secrets.h"

// Debug
#define DEBUG_ENABLED

// PN532 pins
#define PN532_IRQ 4
#define PN532_RST 5 // Must be connected to RSTPDN pin, not RSTO!
#define PN532_SDA 3
#define PN532_SCL 2

// RDM6300 pins
#define RDM6300_SERIAL Serial1
#define RDM6300_RX 10

// Debug serial
#define DEBUG_SERIAL Serial

// Output serial
#define OUTPUT_SERIAL Serial1
#define OUTPUT_SERIAL_SPEED 115200

// Beeper
#define BEEPER_GND 16
#define BEEPER_VCC 35

// WiFi
#define WIFI_SSID "B4CK"
#define WIFI_PASSWORD SECRET_WIFI_PASSWORD

// WiFi
#define OTA_HOSTNAME "acs-reader"
#define OTA_PASSWORD SECRET_OTA_PASSWORD
#define OTA_PORT 3232

#endif