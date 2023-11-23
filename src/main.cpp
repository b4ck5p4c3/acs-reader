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

std::vector<uint32_t> kStartupBeeps {200, 100, 200};

#include "nfc/interface/pn532.h"

#include <memory>

void setup() {
  InitDebug();

  // Init125KHz();
  InitNFC();
  InitWiFi();
  InitArduinoOTA();
  InitBeeper();
  InitOutput();

  // StartWiFi();

  // Start125KHz();
  Beep(kStartupBeeps);
  StartNFC();
}

void loop() {
}