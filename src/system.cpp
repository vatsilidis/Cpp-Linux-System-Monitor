#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }
vector<Process>& System::Processes() { 
    vector<int> pidSet = LinuxParser::Pids();

    for(int pid: pidSet){
       Process p(pid);
       processes_.push_back(p);
   }
std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//store the UpTime to the local uptime variable for not parssing in every call
long int System::UpTime() { 
   uptime = LinuxParser::UpTime();
   return uptime;
}