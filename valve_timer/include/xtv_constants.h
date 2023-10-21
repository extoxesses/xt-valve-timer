#ifndef XT_VALVE_TIMER_CONST_H
#define XT_VALVE_TIMER_CONST_H

// System
const short MAX_VALVES = 4;
const short WEEK_SIZE = 7;

const short SYSTEM_CFG_SIZE = 4;
const short VALVE_CFG_SIZE = 6;

// GPIO pins
const short LCD_RES  = 2;
const short LCD_SCE  = 3;
const short LCD_DC   = 4;
const short LCD_SDIN = 5;
const short LCD_SCK  = 6;

const short RTC_CLK = 7;
const short RTC_DAT = 8;
const short RTC_RST = 9;

const short ENTER_BTN_PIN = 18;
const short RETURN_BTN_PIN = 19;
const short NEXT_BTN_PIN = 20;
const short PREV_BTN_PIN = 21;

const short HW_SWITCH_PIN = 22;
const short DEBUG_PIN = 23;

const short RELAY_VALVE_START_PIN = 30;
const short RELAY_SUPPLY_START_PIN = 40;

// Static configurations
const short JSON_SIZE = 1024;

#endif /* XT_VALVE_TIMER_CONST_H */
