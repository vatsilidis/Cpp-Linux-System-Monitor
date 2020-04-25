#include "processor.h"
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() {
    long wakeJiffies = LinuxParser::ActiveJiffies();
    long sleepJiffies = LinuxParser::IdleJiffies();
    long activeJiffiesTime = wakeJiffies - cachedWakeJiffies;
    long sleepJiffiesTime = sleepJiffies - cachedSleepJiffies;
    long totalJiffiesTime = activeJiffiesTime + sleepJiffiesTime;
    cachedWakeJiffies = wakeJiffies;
    cachedSleepJiffies = cachedSleepJiffies;
    return  static_cast<float>(activeJiffiesTime / totalJiffiesTime);
}

/*
δευτερη περιπτωση
float Processor::Utilization() {
  return (float)LinuxParser::ActiveJiffies() / LinuxParser::Jiffies();
}
*/