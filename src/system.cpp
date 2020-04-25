#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }
vector<Process>& System::Processes() { 
    vector<int> pidSet = LinuxParser::Pids();

    /*
    //create extension set for Pids
    set<int> extensionPids;
    for (Process const& prcss : processes_){
        extensionPids.insert(prcss.Pid());
    }
    
    //emplace the new processes
    for (int pid : pidSet){
        if(extensionPids.find(pid) == extensionPids.end()){
            processes_.emplace_back(pid);
        }
    }
    */
   for(int pid: pidSet){
       
   }
   

    //update cpu utilization with new processes
    for (Process& prcss : processes_){
        prcss.CpuUtilization(LinuxParser::ActiveJiffies(prcss.Pid() / LinuxParser::ActiveJiffies() ));
    }
    
    std::sort(processes_.begin(), processes_.end(), std::greater_equal());
    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//store the UpTime to the local uptime variable for not parssing in every call
long int System::UpTime() { 
    if (uptime==0.0){
        uptime = LinuxParser::UpTime();
        return uptime;
    }else {return uptime;}
}