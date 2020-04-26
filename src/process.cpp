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

Process::Process(int pid) : pid_(pid){
    cpu_util = Process::CpuUtilization();
    totalTime_ = LinuxParser::ActiveJiffies(this->pid_);
    seconds_ = LinuxParser::UpTime(this->pid_);
}

// Return this process's ID
int Process::Pid() const { return  this->pid_; }
//DONE  INT ID of process




// Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(this->pid_); 
}
// DONE   STRING with the command assosiated with process
// Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(this->pid_);
}
// DONE   STRING Memory Utilization / Process ID in MBytes
// Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(this->pid_); 
}
// DONE   STRING username of Input PID
// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(this->pid_); 
}
// DONE  LONG in Seconds of Process UPTIME
// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->cpu_util > a.cpu_util;
}
// DONE  overloaded


// Return this process's CPU utilization
float Process::CpuUtilization() { 
    long seconds = LinuxParser::UpTime() - 
                    Process::UpTime();
    return (float)totalTime_ / seconds;
}
// ************************************************
// LONG seconds from ActiveJiffies(pid_)
//  DIVIDED with
// LONG UpTime(pid)
// must return float percentage of Utilization