#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "constants.h"

// === System properties and configurations ===

typedef struct {
  bool manual = true;
  bool active = false;
  long timerHour = 12;
  long timerMinute = 30;
  long duration = 10;
  bool days[WEEK_SIZE] = {false, false, false, false, false, false, false};
} Valve;
Valve valves[MAX_VALVES];

// DISPLAY configuration
unsigned short contrast = 47;
Adafruit_PCD8544 LCD(D_SCK, D_SDIN, D_DC, D_SCE, D_RES);

// RCT configuration
ThreeWire RTC_WIRE(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> RTC(RTC_WIRE);


// === State machine configurations ===

typedef void (*StateFunction)(Adafruit_PCD8544&, Valve&, short);
typedef void (CallbackFunction)(Adafruit_PCD8544&, Valve&);

const short SYSTEM_CFG_SIZE = 4;
const short VALVE_CFG_SIZE = 6;
short* stateMachineSize;

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&displayLandingScreen, &displaySettingsClockCb, &displaySettingsCalendarCb, &displaySettingsContrastCb};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&displayValveCb, &displayValveManualCb, &displayValveActiveCb, &displayValveTimerCb, &displayValveDurationCb, &displayValveDaysCb};
StateFunction** stateMachine;

short IDX = 0;
short NAV_PTR[3] = {0, 0, 0};

bool refresh;
short lastMinute;

// --- Arduino routines ---

void setup() {
  String msg = "Setup started...";
  info(&msg);

  Serial.begin(9600);
  // Init state machine and system inital state
  initStateMachine(&stateMachine, &stateMachineSize, MAX_VALVES);

  // Init peripherals and GPIO pins
  initDisplay(&LCD, contrast);
  initRTC(&RTC);
  initGPIO();

  // Wait system to be stable, and run with start configuration
  delay(1000);

  msg = "Setup completed";
  info(&msg);
}

void loop() {
  RtcDateTime now = RTC.GetDateTime();
  checkTimer(&now, valves, MAX_VALVES);

  if ((lastMinute != now.Minute()) || refresh) {
    lastMinute = now.Minute();
    LCD.clearDisplay();
    short valveIdx = max(0, NAV_PTR[0] - 1);
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, valves[valveIdx], 0);
    delay(250);
    LCD.display();
    refresh = false;
  }
}
