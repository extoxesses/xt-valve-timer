// --- Timer section ---------------------------

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

// --- Valve section ---------------------------

void displayValve() {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  LCD.print("Valve ");
  LCD.println(smIdxX);

  LCD.setTextSize(1);

  LCD.print("Active: ");
  LCD.println(false);

  LCD.print("Manual: ");
  LCD.println(false);

  LCD.print("Timer:  ");
  LCD.println("18:00");

  LCD.print("Days:   ");
  LCD.println("1,3,5");

  LCD.display();
}

void vs1() {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  
  LCD.print("Valve ");
  LCD.println(smIdxX);

  LCD.print("Manual: ");
  LCD.println(false);

  LCD.display();
}

void vs2() {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  
  LCD.print("Valve ");
  LCD.println(smIdxX);

  LCD.print("Active: ");
  LCD.println(false);

  LCD.display();
}

void vs3() {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  
  LCD.print("Valve ");
  LCD.println(smIdxX);

  LCD.print("Timer:  ");
  LCD.println("18:00");

  LCD.display();
}

void vs4() {
  LCD.setTextColor(BLACK);
  LCD.setTextSize(2);
  
  LCD.print("Valve ");
  LCD.println(smIdxX);

  LCD.print("Days:   ");
  LCD.println("1,3,5");

  LCD.display();
}
