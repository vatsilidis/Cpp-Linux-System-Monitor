#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid){}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return the STRING with the command assosiated with process
string Process::Command() { 
    return LinuxParser::Command(Pid()); 
}

// Return STRING (MBytes) this process's memory utilization  Utilization / Process ID
string Process::Ram() { 
    return LinuxParser::Ram(Pid());
}

// Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid()); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid()); 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a)  { 
    return CpuUtilization() > a.CpuUtilization();
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    long totalTime = LinuxParser::ActiveJiffies(Pid());
  	long seconds = LinuxParser::UpTime() - Process::UpTime();
    return (float)totalTime / seconds;
}
