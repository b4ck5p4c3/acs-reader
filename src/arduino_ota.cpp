#include "mcu_config.h"
#include "utils/debug.h"

#include <ArduinoOTA.h>

TimerHandle_t arduino_ota_timer;

void HandleArduinoOTA(TimerHandle_t) {
  DEBUG_PRINT("OTA handling\n");
  ArduinoOTA.handle();
}

void InitArduinoOTA() {
  ArduinoOTA.setHostname(OTA_HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.setPort(OTA_PORT);

  ArduinoOTA.onStart([]() { xTimerStop(arduino_ota_timer, 0); });
  ArduinoOTA.onError(
      [](ota_error_t error) { xTimerStart(arduino_ota_timer, 0); });

  arduino_ota_timer =
      xTimerCreate("arduino_ota_timer", pdMS_TO_TICKS(3000), pdMS_TO_TICKS(500),
                   nullptr, HandleArduinoOTA);
}

void StartArduinoOTA() {
  ArduinoOTA.begin();
  xTimerStart(arduino_ota_timer, 0);
}

void StopArduinoOTA() {
  xTimerStop(arduino_ota_timer, 0);
  ArduinoOTA.end();
}