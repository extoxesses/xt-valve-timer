#ifndef XT_VALVE_TIMER_VALVE_H
#define XT_VALVE_TIMER_VALVE_H

#include "xtv_constants.h"

class Valve {
  public:
    Valve();

    bool isManual();
    void setManual(bool);
    bool isActive();
    void setActive(bool);
    short getTimerHour();
    void setTimerHour(short);
    short getTimerMinute();
    void setTimerMinute(short);
    int getDuration();
    void setDuration(int);
    bool getDay(int);
    void setDay(int, bool);

  private:
    bool manual;
    bool active;
    short timerHour;
    short timerMinute;
    int duration;
    bool* days;
};

#endif /* XT_VALVE_TIMER_VALVE_H */
