#ifndef XT_VALVE_TIMER_STATE_MACHINE_H
#define XT_VALVE_TIMER_STATE_MACHINE_H

#include "xtv_constants.h"
#include "xtv_types.h"
#include "xtv_utils.h"

class StateMachine {

  public:
   /*
    * Displays the timer landing page
    */
    static void displayLandingScreen(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      RtcDateTime now = settings.getRtc().GetDateTime();
      settings.getLcd().setTextColor(BLACK);

      // Time section, in hh:mm format
      settings.getLcd().setCursor(13, 5);
      settings.getLcd().setTextSize(2);
      char* timeString = XtvUtils::formatTime(now);
      settings.getLcd().println(timeString);

      // Date section, in dd/mm/yyyy format
      settings.getLcd().setCursor(13, 22);
      settings.getLcd().setTextSize(1);
      char* dateString = XtvUtils::formatDate(now);
      settings.getLcd().print(dateString);

      // Valves section
      settings.getLcd().setCursor(3, 35);
      settings.getLcd().setTextSize(1);
      settings.getLcd().print("Valve:");

      short startPos = 43;
      for (short i = 0; i < MAX_VALVES; ++i) {
        settings.getLcd().setCursor(startPos, 35);
        settings.getLcd().setTextSize(1);
        if (valves[i].isActive()) {
          settings.getLcd().setTextColor(WHITE, BLACK);
          settings.getLcd().drawRect(startPos - 1, 34, 7, 9, BLACK);
        }
        settings.getLcd().print(i + 1);
        settings.getLcd().setTextColor(BLACK);
        startPos += 10;
      }

      // Display and flush
      delete timeString;
      delete dateString;
    }

    /*
    * Displays the timer clock settings
    */
    static void displaySettingsClockCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      RtcDateTime now = settings.getRtc().GetDateTime();
      if (0 == inc) {
        char* timeString = XtvUtils::formatTime(now);
        drawSettingsCb(settings.getLcd(), "Clock", timeString);
      } else {
        short minute = now.Minute();
        short hour = now.Hour();
        if (indexes[2] % 2 == 0) {
          hour += inc;
        } else {
          minute += inc;
        }
        RtcDateTime date(now.Year(), now.Month(), now.Day(), hour, minute, now.Second());
        settings.getRtc().SetDateTime(date);
      }
    }

    /*
    * Displays the timer date settings
    */
    static void displaySettingsCalendarCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      RtcDateTime now = settings.getRtc().GetDateTime();
      if (0 == inc) {
        char* dateString = XtvUtils::formatDate(now);
        drawSettingsCb(settings.getLcd(), "Date", dateString);
      } else {
        short day = now.Day();
        short month = now.Month();
        short year = now.Year();
        if (indexes[2] % 3 == 0) {
          day += inc;
        } else if (indexes[2] % 3 == 1) {
          month += inc;
        } else {
          year += inc;
        }
        RtcDateTime date(year, month, day, now.Hour(), now.Minute(), now.Second());
        settings.getRtc().SetDateTime(date);
      }
    }

    /*
    * Displays the timer contrast settings
    */
    static void displaySettingsContrastCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      unsigned short contrast = settings.getContrast();
      if (0 == inc) {
        drawSettingsCb(settings.getLcd(), "Contra.", &String(contrast)[0]);
      } else {
        contrast = contrast + inc;
        settings.setContrast(contrast);
        settings.getLcd().setContrast(contrast);
      }
    }

    /*
    * Displays the valve status in carousel's workflow
    */
    static void displayValveCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve valve = *XtvUtils::getValve(valves, indexes);
      settings.getLcd().setTextColor(BLACK);
      drawValveHeader(settings.getLcd(), indexes[0]);
      
      settings.getLcd().setTextSize(1);
      drawValveManualValue(settings.getLcd(), valve);
      drawValveActiveValue(settings.getLcd(), valve);
      drawValveTimerValue (settings.getLcd(), valve);
      drawValveDaysValue  (settings.getLcd(), valve);
    }

    /*
    * Displays the valve manual value in valve settings' workflow
    */
    static void displayValveManualCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve* valve = XtvUtils::getValve(valves, indexes);
      if(inc == 0) {
        drawValveProperties(settings.getLcd(), *valve, indexes, *drawValveManualValue);
      } else {
        valve->setManual(!valve->isManual());
        valve->setActive(false);
      }
    }

    /*
    * Displays the valve active value in valve settings' workflow
    */
    static void displayValveActiveCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve* valve = XtvUtils::getValve(valves, indexes);
      if(inc == 0) {
        drawValveProperties(settings.getLcd(), *valve, indexes, *drawValveActiveValue);
      } else if(valve->isManual()) {
        valve->setActive(!valve->isActive());
      }
    }

    /*
    * Displays the valve timer value in valve settings' workflow
    */
    static void displayValveTimerCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve* valve = XtvUtils::getValve(valves, indexes);
      if (inc == 0) {
        drawValveProperties(settings.getLcd(), *valve, indexes, *drawValveTimerValue);
      } else if (indexes[2] % 2 == 0) {
        int hour = (24 + valve->getTimerHour() + inc) % 24;
        valve->setTimerHour(hour);
      } else {
        int minutes = (60 + valve->getTimerMinute() + inc) % 60;
        valve->setTimerMinute(minutes);
      }
    }

    /*
    * Displays the valve duration value in valve settings' workflow
    */
    static void displayValveDurationCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve* valve = XtvUtils::getValve(valves, indexes);
      if (inc == 0) {
        drawValveProperties(settings.getLcd(), *valve, indexes, *drawValveDurationValue);
      } else {
        valve->setDuration(valve->getDuration() + inc);
      }
    }

    /*
    * Displays the valve manual value valve settings' workflow
    */
    static void displayValveDaysCb(XtvSettings& settings, Valve* valves, short* indexes, short inc) {
      Valve* valve = XtvUtils::getValve(valves, indexes);
      if (inc == 0) {
        drawValveProperties(settings.getLcd(), *valve, indexes, *drawValveDaysValue);
      } else {
        short idx = indexes[2] % WEEK_SIZE;
        valve->setDay(idx, !valve->getDay(idx));
      }
    }

  private:
    /*
    * Draws common settings page
    */
    static void drawSettingsCb(Adafruit_PCD8544& lcd, String title, char* value) {
      lcd.setTextColor(BLACK);
      lcd.setTextSize(2);
      lcd.println(title);

      lcd.setTextSize(1);
      lcd.println(" ------------ ");
      lcd.println(" Set: ");
      lcd.print("   ");
      lcd.println(value);
      lcd.println("\n ------------ ");
    }

    /*
    * Draws valve pages header
    */
    static void drawValveHeader(Adafruit_PCD8544& lcd, short idx) {
      lcd.setTextSize(2);
      lcd.print("Valve ");
      lcd.println(idx);
    }

    /*
    * Draws valve's manual value
    */
    static void drawValveManualValue(Adafruit_PCD8544& lcd, Valve& valve) {
      lcd.print("Manual: ");
      String s = XtvUtils::parseBoolean(valve.isManual());
      lcd.println(s);
    }

    /*
    * Draws valve's active value
    */
    static void drawValveActiveValue(Adafruit_PCD8544& lcd, Valve& valve) {
      lcd.print("Active: ");
      String s = XtvUtils::parseBoolean(valve.isActive());
      lcd.println(s);
    }

    /*
    * Draws valve's timer value
    */
    static void drawValveTimerValue(Adafruit_PCD8544& lcd, Valve& valve) {
      String hours = (valve.getTimerHour() < 10 ? "0" : "") + String(valve.getTimerHour());
      String minutes = (valve.getTimerMinute() < 10 ? "0" : "") + String(valve.getTimerMinute());
      lcd.println("Timer:  " + hours + ":" + minutes);
    }

    /*
    * Draws valve's duration value
    */
    static void drawValveDurationValue(Adafruit_PCD8544& lcd, Valve& valve) {
      lcd.print("Duration:  ");
      lcd.println(valve.getDuration());
    }

    /*
    * Draws valve's days value
    */
    static void drawValveDaysValue(Adafruit_PCD8544& lcd, Valve& valve) {
      lcd.print("Days: ");

      bool prevFound = false;
      for (short i = 0; i < WEEK_SIZE; ++i) {
        if (valve.getDay(i)) {
          lcd.print(i + 1);
          prevFound = true;
        } else {
          lcd.print('-');
        }
      }

      if (!prevFound) {
        lcd.print("  /");
      }
      lcd.print('\n');
    }

    /*
    * Draws valve properties in valve settings' workflow
    */
    static void drawValveProperties(Adafruit_PCD8544& lcd, Valve& valve, short* indexes, CallbackFunction& func) {
      lcd.setTextColor(BLACK);
      drawValveHeader(lcd, indexes[0]);
      lcd.setTextSize(1);
      lcd.println(" ------------ ");
      func(lcd, valve);
    }

};

#endif /* XT_VALVE_TIMER_STATE_MACHINE_H */
