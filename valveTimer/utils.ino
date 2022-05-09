void checkTimer(RtcDateTime& now, Valve* valves, short valvesSize) {
  for (short i = 0; i < valvesSize; ++i) {
    RtcDateTime startTimer = new RtcDateTime(now.Year(), now.Month(), now.Day(), valves[i].timerHour, valves[i].timerMinute, 0);
    RtcDateTime endTimer = startTimer + valves[i].duration * 60;

    if (valves[i].manual) {
      // If manual, do nothing. Skip check
      continue;
    } else if (startTimer < now && now < endTimer && valves[i].days[now.DayOfWeek() - 1])
      // If "now" is in a scheduled range, open relay only if closed
      if(!valves[i].active) {
        valves[i].active = true;
        digitalWrite(RELAY_START_PIN + i, HIGH);
      }
    } else if (valves[i].active) {
      // If "now" isn't in a scheduled range, close relay only if it is open
      valves[i].active = false;
      digitalWrite(RELAY_START_PIN + i, LOW);
    }
  }
}

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

Valve* getValve() {
  short idx = max(0, NAV_PTR[0] - 1);
  return &valves[idx];
}

void info(String* message) {
  if (HIGH == digitalRead(DEBUG_PIN)) {
    Serial.println(*message);
  }
}

const char* parseBoolean(bool value) {
  return value ? "On" : "Off";
}
