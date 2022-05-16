#ifndef XT_VALVE_TIMER_UTILS_H
#define XT_VALVE_TIMER_UTILS_H

#include "Arduino.h"
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "xtv_valve.h"

class XtvUtils {
  public:
    static void checkTimer(RtcDateTime& now, Valve* valves, short valvesSize) {
      for (short i = 0; i < valvesSize; ++i) {
        RtcDateTime startTimer(now.Year(), now.Month(), now.Day(), valves[i].getTimerHour(), valves[i].getTimerMinute(), 0);
        RtcDateTime endTimer = startTimer + valves[i].getDuration() * 60;

        if (valves[i].isManual()) {
          // If manual, do nothing. Skip check
          continue;
        } else if (startTimer < now && now < endTimer && valves[i].getDay(now.DayOfWeek() - 1)) {
          // If "now" is in a scheduled range, open relay only if closed
          if(!valves[i].isActive()) {
            valves[i].setActive(true);
            digitalWrite(RELAY_START_PIN + i, HIGH);
          }
        } else if (valves[i].isActive()) {
          // If "now" isn't in a scheduled range, close relay only if it is open
          valves[i].setActive(false);
          digitalWrite(RELAY_START_PIN + i, LOW);
        }
      }
    }

    static char* formatDate(const RtcDateTime& now) {
      char* dateString = new char[11];
      sprintf(dateString, "%02u/%02u/%04u", now.Day(), now.Month(), now.Year());
      return dateString;
    }

    static char* formatTime(const RtcDateTime& now) {
      char* dateString = new char[6];
      sprintf(dateString, "%02u:%02u", now.Hour(), now.Minute());
      return dateString;
    }

    static Valve* getValve(Valve* valves, short* indexes) {
      short idx = max(0, indexes[0] - 1);
      return &valves[idx];
    }

    static void info(String& message) {
      if (HIGH == digitalRead(DEBUG_PIN)) {
        Serial.println(message);
      }
    }

    static void logRtcConfiguration(RtcDateTime& compiled, RtcDS1302<ThreeWire>& rtc) {
      Serial.println("=== RTC Configuration ===");
      Serial.println(__DATE__);
      Serial.println(__TIME__);
      Serial.println(compiled);
      Serial.println(rtc.GetDateTime());
      Serial.println("=========================");
    }

    static void onChangeLog(short* indexes, bool refresh, short lastMinute) {
      if (LOW == digitalRead(DEBUG_PIN)) {
        return;
      }

      char navBuffer[28];
      sprintf(navBuffer, "Navigation indexes: [%u,%u,%u]", indexes[0], indexes[1], indexes[2]);
      Serial.println(navBuffer);

      char refreshBuffer[15];
      sprintf(refreshBuffer, "Refresh: %s", parseBoolean(refresh));
      Serial.println(refreshBuffer);
      
      char minBuffer[16];
      sprintf(minBuffer, "Last minute: %i", lastMinute);
      Serial.println(minBuffer);
    }

    static const char* parseBoolean(bool value) {
      return value ? "On" : "Off";
    }

};

#endif /* XT_VALVE_TIMER_UTILS_H */
