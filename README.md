# acs-reader

B4CKSP4CE ACS NFC/EMV/RFID reader based on ESP32(S2), PN532 and RDM6300

Allows to:
- Read UIDs from NFC tags
- Read PANs from EMVCo cards (including VISA, MasterCard, MIR)
- Read UIDs from RFID tags (deperecated and not used for now)

To build the project you also require to create `src/secrets.h` with defines for:
```c
#define SECRET_WIFI_PASSWORD "wifipassword" // wifi password
#define SECRET_OTA_PASSWORD "otapassword" // ota update password
```

Also probably you need to change the `WIFI_SSID` and `OTA_HOSTNAME` defines in `src/mcu_config.h` for your specific environment.
