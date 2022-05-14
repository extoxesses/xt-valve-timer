#ifndef XT_VALVE_TIMER_TYPES_H
#define XT_VALVE_TIMER_TYPES_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>

#include "xtv_settings.h"
#include "xtv_valve.h"

// Type definitions
typedef void (*StateFunction)(XtvSettings&, Valve*, short*, short);
typedef void (CallbackFunction)(Adafruit_PCD8544&, Valve&);

#endif /* XT_VALVE_TIMER_TYPES_H */
