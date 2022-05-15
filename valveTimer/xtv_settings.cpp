#include "src/xtv_settings.h"

XtvSettings::XtvSettings() {
  contrast = 47;
  lcd = new Adafruit_PCD8544(LCD_SCK, LCD_SDIN, LCD_DC, LCD_SCE, LCD_RES);

  ThreeWire rtc_wire(RTC_DAT, RTC_CLK, RTC_RST);
  rtc = new RtcDS1302<ThreeWire>(rtc_wire);
}

unsigned short XtvSettings::getContrast(){
  return contrast;
}

void XtvSettings::setContrast(unsigned short contrast){
  this->contrast = contrast;
}

Adafruit_PCD8544& XtvSettings::getLcd(){
  return *lcd;
}

RtcDS1302<ThreeWire>& XtvSettings::getRtc(){
  return *rtc;
}
