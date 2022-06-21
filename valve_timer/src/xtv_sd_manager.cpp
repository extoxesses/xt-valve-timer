#include "../include/xtv_sd_manager.h"

XtvSdManager::XtvSdManager() {
  // Do nothing
}

void XtvSdManager::initSD() {
  if (SD.begin()) {
    XtvUtils::info("SD: Initialization done.");
  } else {
    XtvUtils::info("SD ERROR: Initialization failed!");
  }
  delay(500);
}

bool XtvSdManager::saveSettings(XtvSettings& settings, Valve* valves, short size) {
  XtvUtils::info("Saving settings...");
  bool status = false;
  
  // Note: not move file name into a constant! Process doesn't work properly
  SD.remove("settings.txt");
  File file = SD.open("settings.txt", FILE_WRITE);

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

bool XtvSdManager::loadSettings(XtvSettings& settings, Valve* valves, short size) {
  String fileName = "settings.txt";
  if (!SD.exists(fileName)) {
    XtvUtils::info("WARN: Unable to load settings file: file not exists");
    return false;
  }

  File file = SD.open(fileName, FILE_READ);
  if (!file) {
    XtvUtils::info("ERROR: Unable to open settings file!");
    return false;
  }
  
  String json = file.readString();
  StaticJsonDocument<2048> document;
  deserializeJson(document, json);

  settings.setContrast(document["settings"]["contrast"].as<int>());
  deserializeValves(document, valves, size);
  file.close();
  
  XtvUtils::info("Settings file load completed!");
  return true;
}

// --- Private methods

void XtvSdManager::toJson(String& json, XtvSettings& settings, Valve* valves, short size) {
  StaticJsonDocument<JSON_SIZE> document;
  serializeSettings(document, settings);

  JsonArray valvesArray = document.createNestedArray("valves");
  for (int i = 0; i < size; ++i) {
    serializeValve(valvesArray, valves[i], i);
  }

  serializeJson(document, json);
}

void XtvSdManager::serializeSettings(StaticJsonDocument<JSON_SIZE>& document, XtvSettings& settings) {
  document["settings"]["contrast"] = settings.getContrast();
}

void XtvSdManager::serializeValve(JsonArray& valvesArray, Valve& valve, short id) {
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

void XtvSdManager::deserializeValves(StaticJsonDocument<JSON_SIZE>& document, Valve* valves, short size) {
  for (int i = 0; i < size; ++i) {
    JsonObject jsonValve = document["valves"][i].as<JsonObject>();
    short id = jsonValve["id"].as<int>();
    deserializeValve(jsonValve, valves[id]);
  }
}

void XtvSdManager::deserializeValve(JsonObject& jsonValve, Valve& valve) {
  valve.setManual(jsonValve["manual"].as<bool>());

  valve.setActive(false);
  valve.setTimerHour(jsonValve["timerHour"].as<int>());
  valve.setTimerMinute(jsonValve["timerMinute"].as<int>());
  valve.setDuration(jsonValve["duration"].as<int>());
  
  JsonArray array = jsonValve["days"];
  for(JsonVariant v : array) {
    valve.setDay(v.as<int>(), true);
  }
}
