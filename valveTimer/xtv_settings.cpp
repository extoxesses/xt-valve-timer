#include "src/xtv_settings.h"

XtvSettings::XtvSettings() : rtc(*(new ThreeWire(RTC_DAT, RTC_CLK, RTC_RST))) {
  contrast = 47;
  lcd = new Adafruit_PCD8544(LCD_SCK, LCD_SDIN, LCD_DC, LCD_SCE, LCD_RES);
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
  return rtc;
}
