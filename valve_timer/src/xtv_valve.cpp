#include "../include/xtv_valve.h"

Valve::Valve() {
  manual = true;
  active = false;
  timerHour = 12;
  timerMinute = 30;
  duration = 10;

  days = new bool[WEEK_SIZE];
  for (short i = 0; i < WEEK_SIZE; ++i) {
    days[i] = false;
  }
}

bool Valve::isManual() {
  return manual;
}

void Valve::setManual(bool manual) {
  this->manual = manual;
}

bool Valve::isActive() {
  return active;
}

void Valve::setActive(bool active) {
  this->active = active;
}

short Valve::getTimerHour() {
  return timerHour;
}

void Valve::setTimerHour(short timerHour) {
  this->timerHour = timerHour;
}

short Valve::getTimerMinute() {
  return timerMinute;
}

void Valve::setTimerMinute(short timerMinute) {
  this->timerMinute = timerMinute;
}

int Valve::getDuration() {
  return duration;
}

void Valve::setDuration(int duration) {
  this->duration = duration;
}

bool Valve::getDay(int idx) {
  return (idx < WEEK_SIZE) ? days[idx] : false;
}

void Valve::setDay(int idx, bool state) {
  if (idx < WEEK_SIZE) {
    days[idx] = state;
  }
}
