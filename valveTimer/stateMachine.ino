// --- Timer section callbacks ---------------------------

void landingScreen(Adafruit_PCD8544& lcd, Valve& v, short op) {
  RtcDateTime now = RTC.GetDateTime();
  LCD.setTextColor(BLACK);

  // Time section, in hh:mm format
  LCD.setCursor(13, 5);
  LCD.setTextSize(2);
  char* timeString = formatTime(&now);
  LCD.println(timeString);

  // Date section, in dd/mm/yyyy format
  LCD.setCursor(13, 22);
  LCD.setTextSize(1);
  char* dateString = formatDate(&now);
  LCD.print(dateString);

  // Valves section
  LCD.setCursor(3, 35);
  LCD.setTextSize(1);
  LCD.print("Valve:");

  short startPos = 43;
  for (short i = 0; i < MAX_VALVES; ++i) {
    LCD.setCursor(startPos, 35);
    LCD.setTextSize(1);
    if (valves[i].active) {
      LCD.setTextColor(WHITE, BLACK);
      LCD.drawRect(startPos - 1, 34, 7, 9, BLACK);
    }
    LCD.print(i + 1);
    LCD.setTextColor(BLACK);
    startPos += 10;
  }

  // Display and flush
  delete timeString;
  delete dateString;
}

void settingsClockCallback(Adafruit_PCD8544& lcd, Valve& v, short op) {
  RtcDateTime now = RTC.GetDateTime();
  if (0 == op) {
    char* timeString = formatTime(&now);
    commonsSettingsCallback("Clock", timeString);
  } else {
    // 00:16:38

//    short minute = now.Minute();
//    if (minute == 60) {
//      minute = 0;
//      now._Hour((now.Hour() + 1) % 24);
//    } else if (minute < 0) {
//      minute = 59;
//      now._hour((24 + now.Hour() - 1) % 24);
//    } else {
//      now.Minute(minute);
//    }
  }
}

void settingsCalendarCallback(Adafruit_PCD8544& lcd, Valve& v, short op) {
  RtcDateTime now = RTC.GetDateTime();
  char* dateString = formatDate(&now);
  commonsSettingsCallback("Date", dateString);
}

void settingsContrastCallback(Adafruit_PCD8544& lcd, Valve& v, short op) {
  if (0 == op) {
    commonsSettingsCallback("Contra.", &String(contrast)[0]);
  } else {
    contrast = contrast + op;
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
void displayValveCallback(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  lcd.setTextColor(BLACK);
  getValveHeader(lcd, NAV_PTR[0]);
  
  lcd.setTextSize(1);
  drawValveManualValue(lcd, valve);
  valveActive(lcd, valve);
  valveTimer (lcd, valve);
  valveDays  (lcd, valve);
}

/*
 * Displays the valve manual value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void valveManualCallback(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
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
void valveActiveCallback(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if((inc != 0) && valve.manual) {
    valve.active = !valve.active;
  } else {
    drawValveProperties(lcd, valve, *valveActive);
  }
}

/*
 * Displays the valve timer value in valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void valveTimerCallback(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
  if (inc == 0) {
    drawValveProperties(lcd, valve, *valveTimer);
  } else if (NAV_PTR[2] % 2 == 0) {
    valve.timerHour = (24 + valve.timerHour + inc) % 24;
  } else {
    valve.timerMinute = (60 + valve.timerMinute + inc) % 60;
  }
}

/*
 * Displays the valve manual value valve settings' workflow
 * 
 * @param lcd   Display reference
 * @param valve Reference valve
 * @param inc   Incremental value to update valve state
 */
void valveDaysCallback(Adafruit_PCD8544& lcd, Valve& valve, short inc) {
   if (inc == 0) {
    drawValveProperties(lcd, valve, *valveDays);
  } else {
    short idx = NAV_PTR[2] % WEEK_SIZE;
    valve.days[idx] = !valve.days[idx];
  }
}

// --- "Private" callback utilities ---------------------------

void commonsSettingsCallback(char* title, char* value) {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  LCD.println(title);

  LCD.println(" ------------ ");
  LCD.println(" Set: ");
  LCD.print("   ");
  LCD.println(value);
  LCD.println("\n ------------ ");
}

// Deprecated
Valve* getValve() {
  short currentValve = NAV_PTR[0];
  return &valves[currentValve];
}

void getValveHeader(Adafruit_PCD8544& lcd, short idx) {
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

void valveActive(Adafruit_PCD8544& lcd, Valve& v) {
  lcd.print("Active: ");
  lcd.println(parseBoolean(v.active));
}

void valveTimer(Adafruit_PCD8544& lcd, Valve& v) {
  String hours = (v.timerHour < 10 ? "0" : "") + String(v.timerHour);
  String minutes = (v.timerMinute < 10 ? "0" : "") + String(v.timerMinute);
  lcd.println("Timer:  " + hours + ":" + minutes);
}

void valveDays(Adafruit_PCD8544& lcd, Valve& v) {
  lcd.print("Days: ");

  bool prevFound = false;
  for (short i = 0; i < WEEK_SIZE; ++i) {
    if (v.days[i]) {
      lcd.print(i + 1);
      prevFound = true;
    }
    if (prevFound) {
      lcd.print(',');
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
 * @param func  Callback function
 */
void drawValveProperties(Adafruit_PCD8544& lcd, Valve& valve, CallbackFunction& func) {
  lcd.setTextColor(BLACK);
  getValveHeader(lcd, NAV_PTR[0]);
  lcd.setTextSize(1);
  lcd.println(" ------------ ");
  func(lcd, valve);
  lcd.println("\n ------------ ");
}
