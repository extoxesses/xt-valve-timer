#ifndef XT_VALVE_TIMER_SD_MANAGER_H
#define XT_VALVE_TIMER_SD_MANAGER_H

#include "ArduinoJson.h"

#include <SD.h>

#include "xtv_settings.h"
#include "xtv_utils.h"
#include "xtv_valve.h"

class XtvSdManager {
  public:
    static void initSD() {
      if (!SD.begin()) {
        XtvUtils::info("SD ERROR: Initialization failed!");
        delay(500);
      }
      XtvUtils::info("SD: Initialization done.");
    }

    static bool saveSettings(XtvSettings& settings, Valve* valves, short size) {
      XtvUtils::info("Saving settings...");
      bool status = false;
      
      // Note: not move file name into a constant! Process doesn't work properly
      SD.remove("settings.json");
      File file = SD.open("settings.json", FILE_WRITE);

      if (file) {
        XtvUtils::info("File correctly opened. Writing json...");
        String json;
        toJson(json, settings, valves, size);
        file.println(json);
        file.close();
        status = true;
      } else {
        XtvUtils::info("ERROR: unable to open file");
      }

      XtvUtils::info("Settings write completed.");
      return status;
    }

    static bool loadSettings(XtvSettings& settings, Valve* valves, short size) {
      File file = SD.open("settings.json", FILE_READ);
      while (file.available()) {
        String json = file.readString();
        DynamicJsonDocument document(JSON_SIZE);
        deserializeJson(document, json);

        settings.setContrast(document["contrast"]);
        deserializeValves(document, valves, size);
      }

      return false;
    }
  
  private:
    static void toJson(String& json, XtvSettings& settings, Valve* valves, short size) {
      StaticJsonDocument<JSON_SIZE> document;
      serializeSettings(document, settings);

      JsonArray valvesArray = document.createNestedArray("valves");
      for (int i = 0; i < size; ++i) {
        serializeValve(valvesArray, valves[i], i);
      }

      serializeJson(document, json);
    }

    static void serializeSettings(StaticJsonDocument<JSON_SIZE>& document, XtvSettings& settings) {
      document["settings"]["contrast"] = settings.getContrast();
    }

    static void serializeValve(JsonArray& valvesArray, Valve& valve, short id) {
      JsonObject jsonValve = valvesArray.createNestedObject();

      jsonValve["id"] = id;
      jsonValve["manual"] = valve.isManual();
      jsonValve["timerHour"] = valve.getTimerHour();
      jsonValve["timerMinute"] = valve.getTimerMinute();
      jsonValve["duration"] = valve.getDuration();

      JsonArray days = jsonValve.createNestedArray("days");
      for (int i = 0; i < WEEK_SIZE; ++i) {
        if (valve.getDay(i)) {
          days.add(i);
        }
      }
    }

    static void deserializeValves (DynamicJsonDocument document, Valve* valves, short size) {
      for (int i = 0; i < size; ++i) {
        short id = document["valves"][i]["id"];
        valves[id].setManual(document["valves"][i]["manual"]);
        valves[id].setActive(false);
        valves[id].setTimerHour(document["valves"][i]["timerHour"]);
        valves[id].setTimerMinute(document["valves"][i]["timerMinute"]);
        valves[id].setDuration(document["valves"][i]["duration"]);
        JsonArray array = document["valves"][i]["days"];
        for(JsonVariant v : array) {
            valves[id].setDay(v.as<int>(), true);
        }
      }
    }

};

#endif /* XT_VALVE_TIMER_SD_MANAGER_H */
