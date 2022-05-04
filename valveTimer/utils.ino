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

  char separator = (now->Second() % 2 == 0) ? ':' : ' ';
  snprintf_P(dateString, stringSize, PSTR("%02u%01c%02u"),
             now->Hour(), separator, now->Minute());
  return dateString;
}

const char* parseBoolean(bool value) {
  return value ? "true" : "false";
}

void refreshAndLog(Adafruit_PCD8544* lcd, short delayMillis) {
  //String msg = "State machine status: {" + String(navPtr[0]) + "," + String(smIdxY) + "]";
  //info(&msg);

  lcd->clearDisplay();
  delay(delayMillis);
  lcd->display();
}

void info(String* message) {
  if (HIGH == digitalRead(DEBUG_PIN)) {
    Serial.println(*message);
  }
}
