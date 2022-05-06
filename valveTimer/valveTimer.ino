#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "constants.h"

// === System properties and configurations ===

typedef struct {
  bool active = false;
  bool manual = false;
  long timerHour = 0;
  long timerMinute = 0;
  bool days[WEEK_SIZE] = {false, false, false, false, false, false, false};
} Valve;
Valve valves[MAX_VALVES];

// DISPLAY configuration
unsigned short contrast = 50;
Adafruit_PCD8544 LCD(D_SCK, D_SDIN, D_DC, D_SCE, D_RES);

// RCT configuration
ThreeWire RTC_WIRE(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> RTC(RTC_WIRE);


// === State machine configurations ===

typedef void (*StateFunction)(Adafruit_PCD8544&, Valve&, short);
typedef void (CallbackFunction)(Adafruit_PCD8544&, Valve&);

const short SYSTEM_CFG_SIZE = 4;
const short VALVE_CFG_SIZE = 5;
short* stateMachineSize;

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&landingScreen, &settingsClockCallback, &settingsCalendarCallback, &settingsContrastCallback};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&displayValveCallback, &valveManualCallback, &valveActiveCallback, &valveTimerCallback, &valveDaysCallback};
StateFunction** stateMachine;

short IDX = 0;
short NAV_PTR[3] = {0, 0, 0};

bool refresh;

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
  if(refresh) {
    LCD.clearDisplay();
    stateMachine[NAV_PTR[0]][NAV_PTR[1]](LCD, valves[NAV_PTR[0]], 0);
    delay(250);
    LCD.display();
    refresh = false;
  }
}
