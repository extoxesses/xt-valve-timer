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
  long timer = 0;
  bool days[WEEK_SIZE] = {false, false, false, false, false, false, false};
} Valve;
Valve valves[MAX_VALVES];

// DISPLAY configuration
unsigned short contrast = 50;
Adafruit_PCD8544 LCD(D_SCK, D_SDIN, D_DC, D_SCE, D_RES);

// RCT configuration
ThreeWire RTC_WIRE(RTC_DAT, RTC_CLK, RTC_RST);
//ThreeWire RTC_WIRE(RTC_RST, RTC_CLK, RTC_DAT);
RtcDS1302<ThreeWire> RTC(RTC_WIRE);


// === State machine configurations ===

/*
   To simplify the state management, each "state function" has to implement an "main-like interface".
   So, a "state function" has to expose two arguments:
   - int argc
   - char *argv[]
*/
typedef void (*StateFunction)();

const short SYSTEM_CFG_SIZE = 1;
const short VALVE_CFG_SIZE = 5;
short* stateMachineSize;

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&landingScreen};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&displayValve, &vs1, &vs2, &vs3, &vs4};
StateFunction** stateMachine;

short smIdxX;
short smIdxY;
bool refresh;

// --- State machine ---
// TODO: Check "bounce" issue

void onEnterRise() {
  if (!refresh) {
    smIdxX = ++smIdxX % (1 + MAX_VALVES);
    refresh = true;
  }
}

void onReturnRise() {
  if (!refresh) {
    smIdxX = (1 + MAX_VALVES + smIdxX - 1) % (1 + MAX_VALVES);
    refresh = true;
  }
}

void onNextRise() {
  if (!refresh) {
    smIdxY = ++smIdxY % 1; // TOD: componentsSize[smIdxX];
    refresh = true;
  }
}

void onPrevRise() {
  if (!refresh) {
    // smIdxY = (componentsSize[smIdxX] + smIdxY - 1) % componentsSize[smIdxX];
    smIdxY = (1 + smIdxY - 1) % 1;
    refresh = true;
  }
}

// --- Arduino routines ---

void setup() {
  // Init peripherals and GPIO pins
  Serial.begin(9600);
  initDisplay(&LCD, contrast);
  initRTC(&RTC);
  initGPIO();

  // Init state machine and system inital state
  initStateMachine(&stateMachine, &stateMachineSize, MAX_VALVES);

  // Wait system to be stable, and run with start configuration
  delay(1000);
  
  String msg = "Setup completed";
  info(&msg[0]);
}

void loop() {
  stateMachine[smIdxX][smIdxY]();
  //VALVE_CFG[0]();

  boolean cmp = SYSTEM_CFG == &(stateMachine[0]);
  Serial.println(cmp);
  
  refreshAndLog(&LCD, 250); // TODO: review delay. Shoud be a second divider
  refresh = false;
}
