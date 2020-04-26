#include <string>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  long HH{seconds/3600};
  long H_re{seconds%3600};
  long MM{H_re/60};
  long SS{H_re%60};
  string HH_str{std::to_string(HH)};
  string MM_str{std::to_string(MM)};
  string SS_str{std::to_string(SS)};
  string time{HH_str+':'+MM_str+':'+SS_str};
  return time;
}


/*
#define HOUR 3600
#define MINUTE 60

string Format::ElapsedTime(long seconds) { 
    int hours_ = seconds / HOUR;
    int seconds_ =  seconds % HOUR;
    int minutes_ =  seconds / MINUTE;
    seconds_ =  seconds % MINUTE;

    string hh, mm, ss;
    hh = (hours_ < 10) ? ("0" + to_string(hours_)) : to_string(hours_);
    mm = (minutes_ < 10) ? ("0" + to_string(minutes_)) : to_string(minutes_);
    ss = (seconds_ < 10) ? ("0" + to_string(seconds_)) : to_string(seconds_);
    return hh + ":" + mm + ":" + ss; 
}
*/