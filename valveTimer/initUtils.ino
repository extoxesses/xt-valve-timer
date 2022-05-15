void initDisplay(Adafruit_PCD8544& lcd, short contrast) {
  lcd.begin();
  lcd.setContrast(contrast);
  lcd.clearDisplay();
}

void initGPIO() {
  // Debug
  pinMode(DEBUG_PIN, INPUT);
  // Relay
  for(short i = 0; i < MAX_VALVES; ++i) {
    pinMode(RELAY_START_PIN, OUTPUT);
    digitalWrite(RELAY_START_PIN + i, LOW);
  }
  // Interrupts
  short mode = FALLING;
  attachInterrupt(digitalPinToInterrupt(ENTER_BTN_PIN),  onEnterRise,  mode);
  attachInterrupt(digitalPinToInterrupt(RETURN_BTN_PIN), onReturnRise, mode);
  attachInterrupt(digitalPinToInterrupt(PREV_BTN_PIN),   onPrevRise,   mode);
  attachInterrupt(digitalPinToInterrupt(NEXT_BTN_PIN),   onNextRise,   mode);
}

void initStateMachine(StateFunction*** stateMachine, short** stateMachineSize, short size) {
  short smSize = 1 + size;
  *stateMachine = new StateFunction*[smSize];
  *stateMachineSize = new short[smSize];
  
  (*stateMachine)[0] = SYSTEM_CFG;
  (*stateMachineSize)[0] = SYSTEM_CFG_SIZE;
  for (int i = 1; i < smSize; ++i) {
      (*stateMachine)[i] = VALVE_CFG;
      (*stateMachineSize)[i] = VALVE_CFG_SIZE;
  }

  refresh = true;
}

void initRTC(RtcDS1302<ThreeWire>& rtc) {
  RtcDateTime compiled(__DATE__, __TIME__);
  rtc.SetIsWriteProtected(false);
  rtc.SetDateTime(compiled);
  rtc.SetIsRunning(true);

  logRtcConfiguration(compiled, rtc);
}

// --- "Private" methods

void logRtcConfiguration(RtcDateTime& compiled, RtcDS1302<ThreeWire>& rtc) {
  Serial.println("=== RTC Configuration ===");
  Serial.println(__DATE__);
  Serial.println(__TIME__);
  Serial.println(compiled);
  Serial.println(rtc.GetDateTime());
  Serial.println("=========================");
}
