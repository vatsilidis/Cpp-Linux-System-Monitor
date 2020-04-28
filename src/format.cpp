#include <string>

#include "format.h"

#define HOUR 3600
#define MIN 60

using std::string;
using std::to_string;

// INPUT: LONG seconds  -   OUTPUT: STRING HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hour, minute, second;
  string hh, mm, ss;
  hour = seconds / HOUR;
  second = seconds % HOUR;
  minute = second / MIN;
  second %= MIN;

  hh = (hour < 10) ? ("0" + to_string(hour)) : to_string(hour);
  mm = (minute < 10) ? ("0" + to_string(minute)) : to_string(minute);
  ss = (second < 10) ? ("0" + to_string(second)) : to_string(second);
  return hh + ":" + mm + ":" + ss;
}
