#ifndef XT_VALVE_TIMER_TYPES_H
#define XT_VALVE_TIMER_TYPES_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>

#include "xtv_settings.h"
#include "xtv_valve.h"

// Types definitions interface

/**
 * State machine function definition
 * 
 * XtvSettings& - Settings object reference
 * Valve*       - Valves array pointer
 * short*       - Navigation indexes array pointer
 * short        - Update increment
 */
typedef void (*StateFunction)(XtvSettings&, Valve*, short*, short);

/**
 * State machine callback function
 *
 * Adafruit_PCD8544& - LCD monitor reference
 * Valve&            - Valve reference
 */
typedef void (CallbackFunction)(Adafruit_PCD8544&, Valve&);

#endif /* XT_VALVE_TIMER_TYPES_H */
