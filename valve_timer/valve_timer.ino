#include "include/xtv_settings.h"
#include "include/xtv_state_machine.h"
#include "include/xtv_types.h"
#include "include/xtv_utils.h"

// --- State machine configurations
StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&StateMachine::displayLandingScreen, &StateMachine::displaySettingsClockCb, &StateMachine::displaySettingsCalendarCb, &StateMachine::displaySettingsContrastCb};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&StateMachine::displayValveCb, &StateMachine::displayValveManualCb, &StateMachine::displayValveActiveCb, &StateMachine::displayValveTimerCb, &StateMachine::displayValveDurationCb, &StateMachine::displayValveDaysCb};

// --- Timer configuration variables
XtvSettings settings;
Valve valves[MAX_VALVES];

short* stateMachineSize;
StateFunction** stateMachine;

short IDX = 0;
short NAV_PTR[3] = {0, 0, 0};

bool refresh;
short lastMinute;

// --- Arduino routines

void setup() {
  Serial.begin(9600);
  Serial.println("Setup started...");
  
  // Init state machine and system inital state
  initStateMachine(&stateMachine, &stateMachineSize, MAX_VALVES);

  // Init GPIO pins
  initGPIO();
  // Init peripherals
  initDisplay(settings.getLcd(), settings.getContrast());
  initRTC(settings.getRtc());

  // Wait system to be stable, and run with start configuration
  delay(1000);
  Serial.println("Setup completed");
}

void loop() {
  RtcDateTime now = settings.getRtc().GetDateTime();
  XtvUtils::checkTimer(now, valves, MAX_VALVES);

  if ((lastMinute != now.Minute()) || refresh) {
    lastMinute = now.Minute();
    settings.getLcd().clearDisplay();
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](settings, valves, NAV_PTR, 0);
    delay(250);
    settings.getLcd().display();
    refresh = false;
  }
}
