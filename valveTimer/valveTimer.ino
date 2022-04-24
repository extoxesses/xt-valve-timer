#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>


// === Constants Section ===

// System
const short MAX_VALVES = 4;
const short WEEK_SIZE = 7;

// GPIO pins
const short D_RES  = 3;
const short D_SCE  = 4;
const short D_DC   = 5;
const short D_SDIN = 6;
const short D_SCK  = 7;

const short RTC_DAT = 9;
const short RTC_CLK = 8;
const short RTC_RST = 10;

const short ENTER_BTN_PIN = 18;
const short RETURN_BTN_PIN = 19;
const short NEXT_BTN_PIN = 20;
const short PREV_BTN_PIN = 21;

const short DEBUG_PIN = 23;

const short RELAY_1 = 30;
const short RELAY_2 = 31;
const short RELAY_3 = 32;
const short RELAY_4 = 33;


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
RtcDS1302<ThreeWire> RTC(RTC_WIRE);


// === State machine configurations ===

/*
   To simplify the state management, each "state function" has to implement an "main-like interface".
   So, a "state function" has to expose two arguments:
   - int argc
   - char *argv[]
*/
typedef void (*StateFunction)();

/* --------------------------------------------------------------------------------
  const short stateMachineSize = 3;
  const short componentsSize[stateMachineSize] = {5, 0, 0};

  const StateFunction carousel[] = {&DISPLAYClock, &DISPLAYValve, &Valve, &displayValve, &displayValve};
  const StateFunction systemConfig[] = {};
  const StateFunction valveConfig[] = {};

  const StateFunction* stateMachine[stateMachineSize] = {carousel, systemConfig, valveConfig};
  // -------------------------------------------------------------------------------- */

short smIdxX;
short smIdxY;
bool refresh;

const short SYSTEM_CFG_SIZE = 1; // TODO
const short VALVE_CFG_SIZE = 1; // TODO

StateFunction SYSTEM_CFG[SYSTEM_CFG_SIZE] = {&landingScreen};
StateFunction VALVE_CFG[VALVE_CFG_SIZE] = {}; // {&displayValve};

const short stateMachineSize = 1 + MAX_VALVES; // TODO: to remove
short componentsSize[1 + MAX_VALVES];
StateFunction* stateMachine[1 + MAX_VALVES];

void landingScreen() {
  RtcDateTime now = RTC.GetDateTime();
  LCD.setTextColor(BLACK);

  LCD.setCursor(13, 5);
  LCD.setTextSize(2);
  char* timeString = formatTime(now);
  LCD.println(timeString);

  LCD.setCursor(13, 22);
  LCD.setTextSize(1);
  char* dateString = formatDate(now);
  LCD.print(dateString);

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

  LCD.display();
  delete timeString;
  delete dateString;
}

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
  LCD.begin();
  LCD.setContrast(contrast);
  LCD.clearDisplay();
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

void initStateMachine() {
  stateMachine[0] = SYSTEM_CFG;
  componentsSize[0] = SYSTEM_CFG_SIZE;
  for (int i = 1; i < (1 + MAX_VALVES); ++i) {
    componentsSize[i] = VALVE_CFG_SIZE;
    stateMachine[i] = VALVE_CFG;
  }

  smIdxX = 0;
  smIdxY = 0;
  refresh = false;
}


// --- Utils ---

char* formatDate(const RtcDateTime now) {
  short arrSize = 11;
  char* dateString = new char[arrSize];
  short stringSize = arrSize * 8; // char sizeOf

  snprintf_P(dateString, stringSize, PSTR("%02u/%02u/%04u"),
             now.Day(), now.Month(), now.Year());
  return dateString;
}

char* formatTime(const RtcDateTime now) {
  short arrSize = 6;
  char* dateString = new char[arrSize];
  short stringSize = arrSize * 8; // char sizeOf

  char separator = (now.Second() % 2 == 0) ? ':' : ' ';
  snprintf_P(dateString, stringSize, PSTR("%02u%01c%02u"),
             now.Hour(), separator, now.Minute());
  return dateString;
}

void refreshAndLog(short delayMillis) {
  String msg = "SM: [" + String(smIdxX) + "," + String(smIdxY) + "]";
  info(&msg[0]);

  LCD.clearDisplay();
  delay(delayMillis);
  LCD.display();
}

void info(char* message) {
  if (HIGH == digitalRead(DEBUG_PIN)) {
    Serial.println(message);
  }
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
  // if (refresh) {

  //    stateMachine[smIdxX][smIdxY]();
  landingScreen();
  refreshAndLog(250); // TODO: review delay. Shoud be a second divider
  // delay(100);
  refresh = false;

  // }
}
