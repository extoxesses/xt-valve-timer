char* formatDate(const RtcDateTime* now) {
  short arrSize = 11;
  char* dateString = new char[arrSize];
  short stringSize = arrSize * 8; // char sizeOf

  snprintf_P(dateString, stringSize, PSTR("%02u/%02u/%04u"),
             now->Day(), now->Month(), now->Year());
  return dateString;
}

char* formatTime(const RtcDateTime* now) {
  short arrSize = 6;
  char* dateString = new char[arrSize];
  short stringSize = arrSize * 8; // char sizeOf

  snprintf_P(dateString, stringSize, PSTR("%02u:%02u"),
             now->Hour(), now->Minute());
  return dateString;
}

const char* parseBoolean(bool value) {
  return value ? "true" : "false";
}

// Deprecated
//void refreshAndLog(Adafruit_PCD8544* lcd, short delayMillis) {
//  lcd->clearDisplay();
//  delay(delayMillis);
//  lcd->display();
//}

void info(String* message) {
  if (HIGH == digitalRead(DEBUG_PIN)) {
    Serial.println(*message);
  }
}
