#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>


// === Constants ===

// System
const short MAX_VALVES = 4;
const short WEEK_SIZE = 7;

// GPIO pins 
const short D_RES  = 3;
const short D_SCE  = 4;
const short D_DC   = 5;
const short D_SDIN = 6;
const short D_SCK  = 7;

const short ENTER_BTN_PIN = 18;
const short RETURN_BTN_PIN = 19;
const short NEXT_BTN_PIN = 20;
const short PREV_BTN_PIN = 21;

const short DEBUG_PIN = 23;

const short RELAY_1 = 30;
const short RELAY_2 = 31;
const short RELAY_3 = 32;
const short RELAY_4 = 33;

// === Properties ===

// System
typedef struct {
  bool active = false;
  bool manual = false;
  long timer = 0;
  bool days[WEEK_SIZE] = {false,false,false,false,false,false,false};
} Valve;
Valve valves[MAX_VALVES];

// Display
unsigned short contrast = 50;
Adafruit_PCD8544 display = Adafruit_PCD8544(D_SCK, D_SDIN, D_DC, D_SCE, D_RES);

// === State machine configurations ===

/*
 * To simplify the state management, each "state function" has to implement an "main-like interface".
 * So, a "state function" has to expose two arguments:
 * - int argc
 * - char *argv[]
*/
typedef void (*StateFunction)();

/* --------------------------------------------------------------------------------
const short stateMachineSize = 3;
const short componentsSize[stateMachineSize] = {5, 0, 0};

const StateFunction carousel[] = {&displayClock, &displayValve, &displayValve, &displayValve, &displayValve};
const StateFunction systemConfig[] = {};
const StateFunction valveConfig[] = {};

const StateFunction* stateMachine[stateMachineSize] = {carousel, systemConfig, valveConfig};
// -------------------------------------------------------------------------------- */

short smIdxX;
short smIdxY;
bool refresh;

const short SYSTEM_CFG_SIZE = 1; // TODO
const short VALVE_CFG_SIZE = 1; // TODO

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&displayClock};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {&displayValve};

const short stateMachineSize = 1 + MAX_VALVES; // TODO: to remove
short componentsSize[1 + MAX_VALVES];
StateFunction* stateMachine[1 + MAX_VALVES];


// --------------------------------------------------------------------------------

// --- State machine ---
// TODO: Check "bounce" issue

void onEnterRise() {
  if (!refresh) {
    smIdxX = ++smIdxX % stateMachineSize;
    refresh = true;
  }
}

void onReturnRise() { 
  if (!refresh) {
    smIdxX = (stateMachineSize + smIdxX - 1) % stateMachineSize;
    refresh = true;
  }
}

void onNextRise() {
  if (!refresh) {
    smIdxY = ++smIdxY % componentsSize[smIdxX];
    refresh = true;
  }
}

void onPrevRise() {
  if (!refresh) {
    smIdxY = (componentsSize[smIdxX] + smIdxY - 1) % componentsSize[smIdxX];
    refresh = true;
  }
}

// --- Init functions ---

void initDisplay() {
  display.begin();
  display.setContrast(contrast);
  display.clearDisplay();
}

void initGPIO() {
  // Debug
  pinMode(DEBUG_PIN, INPUT);
  // Relay
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  // RTC
  //pinMode(DEBUG_PIN, OUTPUT);
  //pinMode(DEBUG_PIN, OUTPUT);
  //pinMode(DEBUG_PIN, OUTPUT);
  //pinMode(DEBUG_PIN, OUTPUT);
  // Interrupts
  short mode = FALLING;
  attachInterrupt(digitalPinToInterrupt(ENTER_BTN_PIN),  onEnterRise,  mode);
  attachInterrupt(digitalPinToInterrupt(RETURN_BTN_PIN), onReturnRise, mode);
  attachInterrupt(digitalPinToInterrupt(PREV_BTN_PIN),   onPrevRise,   mode);
  attachInterrupt(digitalPinToInterrupt(NEXT_BTN_PIN),   onNextRise,   mode);
}

void initStateMachine() {
  stateMachine[0] = SYSTEM_CFG;
  componentsSize[0] = SYSTEM_CFG_SIZE;
  for(int i = 1; i < (1 + MAX_VALVES); ++i) {
    componentsSize[i] = VALVE_CFG_SIZE;
    stateMachine[i] = VALVE_CFG;
  }
  
  smIdxX = 0;
  smIdxY = 0;
  refresh = false;
}

// --- Display subroutines

void refreshAndLog() {
  if (HIGH == digitalRead(DEBUG_PIN)) {
    String msg = "SM: [" + String(smIdxX) + "," + String(smIdxY) + "]";
    Serial.println(msg);
  }

  display.clearDisplay();
  delay(250);
  display.display();
}

void displayClock() {
  display.setTextColor(BLACK);
  
  display.setCursor(13,5);
  display.setTextSize(2);
  display.println("12:01");
  
  display.setCursor(13,22);
  display.setTextSize(1);
  display.print("02/03/2022");

  display.setCursor(3, 35);
  display.setTextSize(1);
  display.print("Valve:");

  /*
  short start = 33;
  for (short i = 0; i < pumps; ++i) {
    start = start + 10;
    // display.drawLine(start, 30, start + 5, 30, BLACK);

    if (i == 2) {
      display.setTextColor(WHITE,BLACK);
      display.drawRect(start - 1, 34, 7, 9, BLACK);
    }
    display.setCursor(start, 35);
    display.setTextSize(1);
    display.print(i + 1);
    
    display.setTextColor(BLACK);
  }*/
  
  display.display();
}

void displayValve() {
  display.setTextColor(BLACK);
  display.setTextSize(2);
  display.print("Valve ");
  display.println(smIdxX);
  
  display.setTextSize(1);

  display.print("Active: ");
  display.println(false);
  
  display.print("Manual: ");
  display.println(false);
  
  display.print("Timer:  ");
  display.println("18:00");
  
  display.print("Days:   ");
  display.println("1,3,5");
  
  display.display();
}

// --- Arduino routines ---

void setup() {
  // Init peripherals and GPIO pins
  Serial.begin(9600);
  initDisplay();
  initGPIO();
  
  // Init state machine and system inital state
  initStateMachine();

  // Wait system to be stable, and run with start configuration
  delay(1000);
  stateMachine[smIdxX][smIdxY]();
}

void loop() {
  // State machine and monitor update
  if (refresh) {
    refresh = false;
    refreshAndLog();
    stateMachine[smIdxX][smIdxY]();
  }
}
