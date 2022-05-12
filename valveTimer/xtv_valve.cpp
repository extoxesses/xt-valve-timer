#include "includes/xtv_valve.h"

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
  changed = true;
}

bool Valve::isActive() {
  return active;
}

void Valve::setActive(bool active) {
  this->active = active;
  changed = true;
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
  changed = true;
}

int Valve::getDuration() {
  return duration;
}

void Valve::setDuration(int duration) {
  this->duration = duration;
  changed = true;
}

bool Valve::getDay(int idx) {
  return (idx < WEEK_SIZE) ? days[idx] : false;
}

void Valve::setDay(int idx, bool state) {
  if (idx < WEEK_SIZE) {
    days[idx] = state;
    changed = true;
  }
}

bool Valve::isChanged() {
  return changed;
}

void Valve::setChanged() {
  this->changed = false;
}
