#include "src/xtv_settings.h"
#include "src/xtv_state_machine.h"
#include "src/xtv_types.h"
#include "src/xtv_utils.h"

// === System properties and configurations ===
XtvSettings settings;
Valve valves[MAX_VALVES];

// === State machine configurations ===
short* stateMachineSize;

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&StateMachine::displayLandingScreen, &StateMachine::displaySettingsClockCb, &StateMachine::displaySettingsCalendarCb, &StateMachine::displaySettingsContrastCb};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&StateMachine::displayValveCb, &StateMachine::displayValveManualCb, &StateMachine::displayValveActiveCb, &StateMachine::displayValveTimerCb, &StateMachine::displayValveDurationCb, &StateMachine::displayValveDaysCb};
StateFunction** stateMachine;

short IDX = 0;
short NAV_PTR[3] = {0, 0, 0};

bool refresh;
short lastMinute;

// --- Arduino routines ---

void setup() {
  String msg = "Setup started...";
  XtvUtils::info(&msg);

  Serial.begin(9600);
  // Init state machine and system inital state
  initStateMachine(&stateMachine, &stateMachineSize, MAX_VALVES);

  // Init peripherals and GPIO pins
  initDisplay(&settings.getLcd(), settings.getContrast());
  initRTC(&settings.getRtc());
  initGPIO();

  // Wait system to be stable, and run with start configuration
  delay(1000);

  msg = "Setup completed";
  XtvUtils::info(&msg);
}

void loop() {
  RtcDateTime now = settings.getRtc().GetDateTime();
  Serial.println(now);
  XtvUtils::checkTimer(&now, valves, MAX_VALVES);

  if ((lastMinute != now.Minute()) || refresh) {
    lastMinute = now.Minute();
    settings.getLcd().clearDisplay();
    short valveIdx = max(0, NAV_PTR[0] - 1);
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](settings, valves, NAV_PTR, 0);
    delay(250);
    settings.getLcd().display();
    refresh = false;
  }
}
