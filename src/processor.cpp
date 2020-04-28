#include "processor.h"
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// Return FLOAT for % percentage of CPU utilization
float Processor::Utilization() {
  return (float)LinuxParser::ActiveJiffies() / LinuxParser::Jiffies();
}
//DONE