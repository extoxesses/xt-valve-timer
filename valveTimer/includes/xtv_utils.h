#ifndef XT_VALVE_TIMER_UTILS_H
#define XT_VALVE_TIMER_UTILS_H

#include <RtcDS1302.h>
#include "xtv_valve.h"

class XtvUtils {
  public:
    static void checkTimer(RtcDateTime& now, Valve* valves, short valvesSize);
    static char* formatDate(const RtcDateTime& now);
    static char* formatTime(const RtcDateTime& now);
    static Valve* getValve();
    static const char* parseBoolean(bool value);
};

#endif /* XT_VALVE_TIMER_UTILS_H */
