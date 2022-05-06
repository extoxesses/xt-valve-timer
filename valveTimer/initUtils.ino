void initDisplay(Adafruit_PCD8544* lcd, short contrast) {
  lcd->begin();
  lcd->setContrast(contrast);
  lcd->clearDisplay();
}

void initGPIO() {
  // Debug
  pinMode(DEBUG_PIN, INPUT);
  // Relay
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
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

void initRTC(RtcDS1302<ThreeWire>* rtc) {
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.println(__TIME__);
  rtc->SetDateTime(compiled);
  rtc->SetIsWriteProtected(false);
  rtc->SetIsRunning(true);
}
