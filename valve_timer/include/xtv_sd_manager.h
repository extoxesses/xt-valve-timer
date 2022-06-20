#ifndef XT_VALVE_TIMER_SD_MANAGER_H
#define XT_VALVE_TIMER_SD_MANAGER_H

#include "ArduinoJson.h"

#include <SD.h>

#include "xtv_settings.h"
#include "xtv_utils.h"
#include "xtv_valve.h"

class XtvSdManager {
  public:
    XtvSdManager();

    void initSD();
    bool saveSettings(XtvSettings& settings, Valve* valves, short size);
    bool loadSettings(XtvSettings& settings, Valve* valves, short size);
  
  private:
    void toJson(String& json, XtvSettings& settings, Valve* valves, short size);
    void serializeSettings(StaticJsonDocument<JSON_SIZE>& document, XtvSettings& settings);
    void serializeValve(JsonArray& valvesArray, Valve& valve, short id);
    void deserializeValves(DynamicJsonDocument& document, Valve* valves, short size);
    void deserializeValve(JsonObject& jsonValve, Valve& valve);

};

#endif /* XT_VALVE_TIMER_SD_MANAGER_H */
