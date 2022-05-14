#include "src/xtv_settings.h"

XtvSettings::XtvSettings() {
  this->contrast = 47;
  this->lcd = new Adafruit_PCD8544(D_SCK, D_SDIN, D_DC, D_SCE, D_RES);

  ThreeWire rtc_wire(RTC_DAT, RTC_CLK, RTC_RST);
  this->rtc = new RtcDS1302<ThreeWire>(rtc_wire);
}

unsigned short XtvSettings::getContrast(){
  return this->contrast;
}

void XtvSettings::setContrast(unsigned short constrast){
  this->contrast = constrast;
}

Adafruit_PCD8544& XtvSettings::getLcd(){
  return *this->lcd;
}

RtcDS1302<ThreeWire>& XtvSettings::getRtc(){
  return *this->rtc;
}