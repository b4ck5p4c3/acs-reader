#include <Arduino.h>

#include "mcu_config.h"
#include "utils/debug.h"
#include "125khz/handler.h"
#include "nfc/handler.h"
#include "esp_wifi.h"
#include "arduino_ota.h"
#include "utils/beeper.h"
#include "utils/output.h"

TaskHandle_t _125khz_task;
TaskHandle_t nfc_task;

void Main125KHzTask(void*) {
  Handle125KHz();
}

void MainNFCTask(void*) {
  HandleNFC();
}

void Start125KHz() {
  xTaskCreatePinnedToCore(Main125KHzTask, 
      "125khz_task", 10000, nullptr,      
      1, &_125khz_task, 1);        
}

void StartNFC() {
  xTaskCreatePinnedToCore(MainNFCTask, 
      "nfc_task", 10000, nullptr,      
      5, &_125khz_task, 0);        
}

const uint32_t kStartupBeeps[] = {200, 100, 200};

void StartupBeepTask(void*) {
  
}

#include "nfc/interface/pn532.h"

#include <memory>

void setup() {
  InitDebug();

  std::unique_ptr<NFCInterface> interface(new PN532(Wire, PN532_SCL, PN532_SDA, PN532_IRQ, PN532_RST));
  DEBUG_PRINT("Init: %d\n", interface->Init());

  while (true) {
    NFCTagInfo info;
    if (interface->FindTag(info, 10000)) {
      DEBUG_PRINT("Read tag: %04x %02x\nUID: ", info.atqa, info.sak);
      for (const auto byte : info.uid) {
        DEBUG_PRINT("%02x ", byte);
      }
      DEBUG_PRINT("\nATS: ");
      for (const auto byte : info.ats) {
        DEBUG_PRINT("%02x ", byte);
      }
      DEBUG_PRINT("\n");
      std::vector<uint8_t> data;
      if (interface->ApduExchange({ 0x00, 0xA4, 0x04, 0x00, 0x0e, 0x32, 0x50, 0x41, 0x59, 0x2e, 0x53, 0x59, 0x53, 0x2e, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00 }, data, 1000)) {
        DEBUG_PRINT("APDU response: ");
        for (const auto byte : data) {
          DEBUG_PRINT("%02x ", byte);
        }
        DEBUG_PRINT("\n");
      } else {
        DEBUG_PRINT("APDU failed\n");
      }
    } else {
      DEBUG_PRINT("Failed to read\n");
    }
  }

  return;
  // Init125KHz();
  InitNFC();
  InitWiFi();
  InitArduinoOTA();
  InitBeeper();
  InitOutput();

  // StartWiFi();

  // Start125KHz();
  Beep(kStartupBeeps, sizeof(kStartupBeeps) / sizeof(kStartupBeeps[0]));
  StartNFC();
}

void loop() {
}