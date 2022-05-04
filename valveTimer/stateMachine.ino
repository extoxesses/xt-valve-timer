// --- Timer section utilities ---------------------------

void commonsSettingsCallback(char* title, char* value) {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  LCD.println(title);

  bar(&LCD, false);
  LCD.println(" Set: ");
  LCD.print("   ");
  LCD.println(value);
  bar(&LCD, true);
  
  LCD.display();
}

// --- Timer section callbacks ---------------------------

void landingScreen() {
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
  LCD.display();
  delete timeString;
  delete dateString;
}

void settingsClockCallback() {
  RtcDateTime now = RTC.GetDateTime();
  char* timeString = formatTime(&now);
  commonsSettingsCallback("Clock", timeString);
}

void settingsCalendarCallback() {
  RtcDateTime now = RTC.GetDateTime();
  char* dateString = formatDate(&now);
  commonsSettingsCallback("Date", dateString);
}

void settingsContrastCallback() {
  RtcDateTime now = RTC.GetDateTime();
  char* dateString = formatDate(&now);
  commonsSettingsCallback("Contra.", &String(contrast)[0]);
}

// --- Valve section utilities ---------------------------

Valve* getValve() {
  short currentValve = NAV_PTR[0];
  return &valves[currentValve];
}

void getValveHeader(Adafruit_PCD8544* lcd, short idx) {
  lcd->setTextSize(2);
  lcd->print("Valve ");
  lcd->println(idx);
}

void valveManual(Adafruit_PCD8544* lcd, Valve* v, float txtSize) {
  lcd->setTextSize(txtSize);
  lcd->print("Manual: ");
  lcd->println(parseBoolean(v->manual));
}

void valveActive(Adafruit_PCD8544* lcd, Valve* v, float txtSize) {
  lcd->setTextSize(txtSize);
  lcd->print("Active: ");
  lcd->println(parseBoolean(v->active));
}

void valveTimer(Adafruit_PCD8544* lcd, Valve* v, float txtSize) {
  lcd->setTextSize(txtSize);
  String hours = (v->timerHour < 10 ? "0" : "") + String(v->timerHour);
  String minutes = (v->timerMinute < 10 ? "0" : "") + String(v->timerMinute);
  lcd->println("Timer:  " + hours + ":" + minutes);
}

void valveDays(Adafruit_PCD8544* lcd, Valve* v, float txtSize) {
  lcd->setTextSize(txtSize);
  lcd->print("Days: ");

  bool prevFound = false;
  for (short i = 0; i < WEEK_SIZE; ++i) {
    if (v->days[i]) {
      lcd->print(i + 1);
      prevFound = true;
    }
    if (prevFound) {
      lcd->print(',');
    }
  }

  if (!prevFound) {
    lcd->print("  /");
  }
  lcd->print('\n');
}

void bar(Adafruit_PCD8544* lcd, bool addSpace) {
  lcd->setTextSize(1);
  if(addSpace) {
    lcd->println("");
  }
  lcd->println(" ------------ ");
}

void valveSettingsCallback(CallbackFunction& func) {
  Valve* v = getValve();
  LCD.setTextColor(BLACK);
  getValveHeader(&LCD, NAV_PTR[0]);
  bar(&LCD, false);
  func(&LCD, v, 1.5);
  bar(&LCD, true);
  LCD.display();
}

// --- Valve section callbacks ---------------------------

void displayValve() {
  Valve *v = getValve();

  LCD.setTextColor(BLACK);
  getValveHeader(&LCD, NAV_PTR[0]);

  valveManual(&LCD, v, 1);
  valveActive(&LCD, v, 1);
  valveTimer(&LCD, v, 1);
  valveDays(&LCD, v, 1);

  LCD.display();
}

void valveManualCallback() {
  valveSettingsCallback(*valveManual);
}

void valveActiveCallback() {
  valveSettingsCallback(*valveActive);
}

void valveTimerCallback() {
  valveSettingsCallback(*valveTimer);
}

void valveDaysCallback() {
  valveSettingsCallback(*valveDays);
}
