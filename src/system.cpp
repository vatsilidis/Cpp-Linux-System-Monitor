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
//DONE return a Reference to a Processor object


// Return Vector<Process> with the ID's from { vector<int> LinuxParser::Pids()  }
vector<Process>& System::Processes() { 
	processes_.clear();
    vector<int> pidSet = LinuxParser::Pids();
    for(int pid: pidSet){
       Process p(pid);
       processes_.emplace_back(p);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}
//DONE


std::string System::Kernel() { return LinuxParser::Kernel(); }
//DONE  STRING with name of Kernel

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
//DONE  FLOAT (Percentage % of memory utilization)

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
//DONE  STRING with name of Operating System

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
//DONE  INT NUMBER of the running processes

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }
//DONE  INT of the total processes

//store the UpTime to the local uptime variable for not parssing in every call
long int System::UpTime() { 
	return LinuxParser::UpTime();
}
//DONE  LONG time in OF SYSTEM UPTIME in SECONDS
