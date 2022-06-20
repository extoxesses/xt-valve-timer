/// void XtvSdManager::saveSettings(XtvSettings& settings, Valve* valves, short size) {
//   XtvUtils::info("Saving settings...");
//   SD.remove(SETTINGS_FILE_NAME);
//   File file = SD.open(SETTINGS_FILE_NAME, FILE_WRITE);
//   if (file) {
//     XtvUtils::info("File correctly opened. Writing json...");
//     String json;
//     this->toJson(json, settings, valves, size);
//     file.println(json);
//     file.close();
//   } else {
//     XtvUtils::info("ERROR: unable to open file");
//   }
//   XtvUtils::info("Settings write completed.");
// }

// // --- Private methods

// void XtvSdManager::toJson(String& json, XtvSettings& settings, Valve* valves, short size) {
//   StaticJsonDocument<JSON_SIZE> document;
//   this->serializeSettings(document, settings);

//   JsonArray valvesArray = document.createNestedArray("valves");
//   for (int i = 0; i < size; ++i) {
//     this->serializeValve(valvesArray, valves[i], i);
//   }

//   serializeJson(document, json);
// }

// void XtvSdManager::serializeSettings(StaticJsonDocument<JSON_SIZE>& document, XtvSettings& settings) {
//   document["settings"]["contrast"] = settings.getContrast();
// }

// void XtvSdManager::serializeValve(JsonArray& valvesArray, Valve& valve, short id) {
//   JsonObject jsonValve = valvesArray.createNestedObject();

//   jsonValve["id"] = id;
//   jsonValve["manual"] = valve.isManual();
//   jsonValve["timerHour"] = valve.getTimerHour();
//   jsonValve["timerMinute"] = valve.getTimerMinute();
//   jsonValve["duration"] = valve.getDuration();

//   JsonArray days = jsonValve.createNestedArray("days");
//   for (int i = 0; i < WEEK_SIZE; ++i) {
//     if (valve.getDay(i)) {
//       days.add(i);
//     }
//   }
// }






// // void XtvSdManager::readSettings(XtvSettings& settings) {
// //   const String fileName = "settings.txt"; // TODO: move to constant
// //   File file = SD.open(fileName, FILE_WRITE);
// //   if (file) {
// //     this->readSettingsFile(file, settings);
// //     file.close();
// //   }
// // }

// // void XtvSdManager::readValve() { }

// // void XtvSdManager::writeSettings(XtvSettings& settings) {
// //   const String fileName = "settings.txt"; // TODO: move to constant
// //   File file = SD.open(fileName, FILE_WRITE);
// //   if (file) {
// //     this->writeSettingsFile(file, settings);
// //     file.close();
// //   }
// // }

// // void XtvSdManager::writeValve() {

// // }

// // // --- Private methods

// // void XtvSdManager::readSettingsYaml(File& file, XtvSettings& settings) {
// //   while (file.available()) {
// //     String raw = file.readString();
// //     unsigned int index = raw.indexOf(':') + 1;
// //     if (index < raw.length()) {
// //       String key = raw.substring(0, index);
// //       String value = raw.substring(index, raw.length());
// //       String contrastKey = "contrast"; // TODO: move to constants
// //       if(contrastKey.equals(key)) {
// //         settings.setContrast(value.toInt());
// //       }
// //     }
// //   }
// // }

// // void XtvSdManager::writeSettingsYaml(File& file, XtvSettings& settings) {
// //   file.println("settings: ");
// //   file.print("  contrast: ");
// //   file.println(settings.getContrast());
// // }

// // void XtvSdManager::writeValveYaml(File& file, XtValve& valve, int id) {
// //   file.println("valve: ");

// //   file.print("  id: ");
// //   file.println(id);  
// //   file.print("  manual: ");
// //   file.println(valve.isManual());
// //   file.print("  timerHour: ");
// //   file.println(valve.getTimerHour());
// //   file.print("  timerMinute: ");
// //   file.println(valve.getTimerMinute());
// //   file.print("  duration: ");
// //   file.println(valve.getDuration());
// //   file.print("  days: ");
// //   for (short i = 0; i < WEEK_SIZE; ++i) {
// //     if (valve.getDay(i)) {
// //       file.print(i);
// //       file.print(',');
// //     }
// //   }
// // }