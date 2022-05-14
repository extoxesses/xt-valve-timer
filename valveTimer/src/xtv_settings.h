#ifndef XT_VALVE_TIMER_SETTINGS_H
#define XT_VALVE_TIMER_SETTINGS_H

#include "Arduino.h"

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "xtv_constants.h"

class XtvSettings {
  public:
    XtvSettings();

    unsigned short getContrast();
    void setContrast(unsigned short);

    Adafruit_PCD8544& getLcd();
    RtcDS1302<ThreeWire>& getRtc();

  private:
    unsigned short contrast;
    Adafruit_PCD8544* lcd;
    RtcDS1302<ThreeWire>* rtc;
};

#endif /* XT_VALVE_TIMER_SETTINGS_H */
