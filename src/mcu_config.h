#ifndef MCU_CONFIG_H_
#define MCU_CONFIG_H_

#include "secrets.h"

// Debug
#define DEBUG_ENABLED

// PN532 pins
#define PN532_IRQ 5
#define PN532_RST 18 // Must be connected to RSTPDN pin, not RSTO!
#define PN532_SDA 26
#define PN532_SCL 25

// RDM6300 pins
#define RDM6300_SERIAL Serial1
#define RDM6300_RX 23

// Serials
#define DEBUG_SERIAL Serial
#define OUTPUT_SERIAL Serial2
#define OUTPUT_SERIAL_SPEED 115200

// WiFi
#define WIFI_SSID "B4CK"
#define WIFI_PASSWORD SECRET_WIFI_PASSWORD

// WiFi
#define OTA_HOSTNAME "acs-reader"
#define OTA_PASSWORD SECRET_OTA_PASSWORD
#define OTA_PORT 3232

#endif