// --- Timer section callbacks ---------------------------

/*
 * Displays the timer landing page
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayLandingScreen(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  RtcDateTime now = RTC.GetDateTime();
  Serial.println(now);
  lcd.setTextColor(BLACK);

  // Time section, in hh:mm format
  lcd.setCursor(13, 5);
  lcd.setTextSize(2);
  char* timeString = formatTime(&now);
  Serial.println(timeString);
  lcd.println(timeString);

  // Date section, in dd/mm/yyyy format
  lcd.setCursor(13, 22);
  lcd.setTextSize(1);
  char* dateString = formatDate(&now);
  Serial.println(dateString);
  lcd.print(dateString);

  // Valves section
  lcd.setCursor(3, 35);
  lcd.setTextSize(1);
  lcd.print("Valve:");

  short startPos = 43;
  for (short i = 0; i < MAX_VALVES; ++i) {
    lcd.setCursor(startPos, 35);
    lcd.setTextSize(1);
    if (valves[i].active) {
      lcd.setTextColor(WHITE, BLACK);
      lcd.drawRect(startPos - 1, 34, 7, 9, BLACK);
    }
    lcd.print(i + 1);
    lcd.setTextColor(BLACK);
    startPos += 10;
  }

  // Display and flush
  delete timeString;
  delete dateString;
}

/*
 * Displays the timer clock settings
 * 
 * @param lcd   Display reference
 * @param valve Valve reference
 * @param inc   Incremental value to update state
 */
void displaySettingsClockCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  RtcDateTime now = RTC.GetDateTime();
  if (0 == inc) {
    char* timeString = formatTime(&now);
    drawSettingsCb(lcd, "Clock", timeString);
  } else {
    short minute = now.Minute();
    short hour = now.Hour();
    if (NAV_PTR[2] % 2 == 0) {
      minute += inc;
    } else {
      hour += inc;
    }
    RtcDateTime date = new RtcDateTime(now.Year(), now.Month(), now.Day(), hour, minute, now.Second());
    RTC.SetDateTime(date);
  }
}

/*
 * Displays the timer date settings
 * 
 * @param lcd   Display reference
 * @param valve Valve reference
 * @param inc   Incremental value to update state
 */
void displaySettingsCalendarCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  RtcDateTime now = RTC.GetDateTime();
  if (0 == inc) {
    char* dateString = formatDate(&now);
    drawSettingsCb(lcd, "Date", dateString);
  } else {
    short day = now.Day();
    short month = now.Month();
    short year = now.Year();
    if (NAV_PTR[2] % 3 == 0) {
      day += inc;
    } else if (NAV_PTR[2] % 3 == 1) {
      month += inc;
    } else {
      year += inc;
    }
    RtcDateTime date = new RtcDateTime(year, month, day, now.Hour(), now.Minute(), now.Second());
    RTC.SetDateTime(date);
  }
}

/*
 * Displays the timer contrast settings
 * 
 * @param lcd   Display reference
 * @param valve Valve reference
 * @param inc   Incremental value to update state
 */
void displaySettingsContrastCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if (0 == inc) {
    drawSettingsCb(lcd, "Contra.", &String(contrast)[0]);
  } else {
    contrast = contrast + inc;
    lcd.setContrast(contrast);
  }
}

// --- Valve section callbacks ---------------------------

/*
 * Displays the valve status in carousel's workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  lcd.setTextColor(BLACK);
  drawValveHeader(lcd, NAV_PTR[0]);
  
  lcd.setTextSize(1);
  drawValveManualValue(lcd, valve);
  drawValveActiveValue(lcd, valve);
  drawValveTimerValue (lcd, valve);
  drawValveDaysValue  (lcd, valve);
}

/*
 * Displays the valve manual value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveManualCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if(inc != 0) {
    valve.manual = !valve.manual;
    valve.active = false;
  } else {
    drawValveProperties(lcd, valve, *drawValveManualValue);
  }
}

/*
 * Displays the valve active value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveActiveCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if((inc != 0) && valve.manual) {
    valve.active = !valve.active;
  } else {
    drawValveProperties(lcd, valve, *drawValveActiveValue);
  }
}

/*
 * Displays the valve timer value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveTimerCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if (inc == 0) {
    drawValveProperties(lcd, valve, *drawValveTimerValue);
  } else if (NAV_PTR[2] % 2 == 0) {
    valve.timerHour = (24 + valve.timerHour + inc) % 24;
  } else {
    valve.timerMinute = (60 + valve.timerMinute + inc) % 60;
  }
}

/*
 * Displays the valve duration value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveDurationCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if (inc == 0) {
    drawValveProperties(lcd, valve, *drawValveDurationValue);
  } else {
    valve.duration += inc;
  }
}

/*
 * Displays the valve manual value valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void displayValveDaysCb(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
   if (inc == 0) {
    drawValveProperties(lcd, valve, *drawValveDaysValue);
  } else {
    short idx = NAV_PTR[2] % WEEK_SIZE;
    valve.days[idx] = !valve.days[idx];
  }
}

// --- "Private" callback utilities ---------------------------

/*
 * Draws common settings page
 * 
 * @param lcd   Display reference
 * @param title Page title
 * @param value Value to display
 */
void drawSettingsCb(Adafruit_PCD8544& lcd, char* title, char* value) {
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
 * 
 * @param lcd LCD diplay reference
 * @param idx Valve index
 */
void drawValveHeader(Adafruit_PCD8544& lcd, short idx) {
  lcd.setTextSize(2);
  lcd.print("Valve ");
  lcd.println(idx);
}

/*
 * Draws valve's manual value
 * 
 * @param lcd     LCD diplay reference
 * @param valve   Reference valve
 * @param txtSize Text size
 */
void drawValveManualValue(Adafruit_PCD8544& lcd, Valve& valve) {
  lcd.print("Manual: ");
  lcd.println(parseBoolean(valve.manual));
}

/*
 * Draws valve's active value
 * 
 * @param lcd     LCD diplay reference
 * @param valve   Reference valve
 * @param txtSize Text size
 */
void drawValveActiveValue(Adafruit_PCD8544& lcd, Valve& valve) {
  lcd.print("Active: ");
  lcd.println(parseBoolean(valve.active));
}

/*
 * Draws valve's timer value
 * 
 * @param lcd     LCD diplay reference
 * @param valve   Reference valve
 * @param txtSize Text size
 */
void drawValveTimerValue(Adafruit_PCD8544& lcd, Valve& valve) {
  String hours = (valve.timerHour < 10 ? "0" : "") + String(valve.timerHour);
  String minutes = (valve.timerMinute < 10 ? "0" : "") + String(valve.timerMinute);
  lcd.println("Timer:  " + hours + ":" + minutes);
}

/*
 * Draws valve's duration value
 * 
 * @param lcd     LCD diplay reference
 * @param valve   Reference valve
 * @param txtSize Text size
 */
void drawValveDurationValue(Adafruit_PCD8544& lcd, Valve& valve) {
  lcd.print("Duration:  ");
  lcd.println(valve.duration);
}

/*
 * Draws valve's days value
 * 
 * @param lcd     LCD diplay reference
 * @param valve   Reference valve
 * @param txtSize Text size
 */
void drawValveDaysValue(Adafruit_PCD8544& lcd, Valve& valve) {
  lcd.print("Days: ");

  bool prevFound = false;
  for (short i = 0; i < WEEK_SIZE; ++i) {
    if (valve.days[i]) {
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
 * 
 * @param lcd   Display reference
 * @param valve Valve reference
 * @param func  Cb function
 */
void drawValveProperties(Adafruit_PCD8544& lcd, Valve& valve, CallbackFunction& func) {
  lcd.setTextColor(BLACK);
  drawValveHeader(lcd, NAV_PTR[0]);
  lcd.setTextSize(1);
  lcd.println(" ------------ ");
  func(lcd, valve);
  lcd.println("\n ------------ ");
}
