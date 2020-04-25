#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <string>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    float memFree, memTotal, value;
    string key, line;
    std::ifstream memstream(kMeminfoFilename);
    if (memstream.is_open()) {
      while (std::getline(memstream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if(key == "MemTotal"){
            memTotal =  value / 1024;
          }
          if(key == "MemFree"){
            memFree =  value / 1024;
            break;
          }
        }
      }
    }
  return (memTotal - memFree) / memTotal; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime; 
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long utime, stime, cutime, cstime, starttime, total_time, 
  seconds, cpu_usage, uptime;
  long int Hz = sysconf(_SC_CLK_TCK);
  string value, line;
  vector<string> value_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
      
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    //hold all values from process file in a string vector
    while(linestream >> value){
      value_list.push_back(value);
    }
    uptime = std::stol(value_list[1]); 
    utime = std::stol(value_list[14]);
    stime = std::stol(value_list[15]);
    cutime = std::stol(value_list[16]);
    cstime = std::stol(value_list[17]);
    starttime= std::stol(value_list[22]);
    //total time spent for the process including the time from children processes
    total_time = utime + stime + cutime + cstime;
    //get the total elapsed time in seconds since the process started
    seconds = uptime - (starttime / Hz);
    //calculate the CPU usage percentage
    cpu_usage = 100 * (total_time / Hz) / seconds;
  }
  return cpu_usage;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long user, nice, system, idle, iowait, irq, softirq, steal; 
  long ActiveJ;
  string value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    //hold all values from process file in a string vector
    while(linestream >> value >> user >> nice >> system
     >> idle >> iowait >> irq >> softirq >> steal){
       if(value == "cpu"){
         ActiveJ = user + nice + system + irq + softirq + steal;
         break;
       }
    }
  }
  return ActiveJ / sysconf(_SC_CLK_TCK);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {  
  long user, nice, system, idle, iowait, irq, softirq, steal; 
  long iddleJ;
  string value, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    //hold all values from process file in a string vector
    while(linestream >> value >> user >> nice >> system
     >> idle >> iowait >> irq >> softirq >> steal){
       if(value == "cpu"){
         iddleJ = idle + iowait;
         break;
       }
    }
  }
  return iddleJ / sysconf(_SC_CLK_TCK);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line, key;
  vector<string> cpuUtil;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    //while(std::getline(stream, line)){
    std::getline(stream, line);
  	std::istringstream linestream(line);
   	while (linestream >> key){
    	if (key != "cpu"){
          cpuUtil.push_back(key);}
    }
  }
  return cpuUtil;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProc = 0;
  int value;
  string key, line;
  std::ifstream stream(kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == "processes"){
          totalProc = value;
          return totalProc;
        }
      }
    }
  }
  return totalProc; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningProc = 0;
  int value;
  string key;
  string line;
  std::ifstream stream(kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if(key == "procs_running"){
          runningProc = value;
          return runningProc;
        }
      }
    }
  }
  return runningProc; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  int vmsize;
  string key, value, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          vmsize = stoi(value) / 1000;
          return to_string(vmsize);
        }
      }
    }
  }
  return to_string(vmsize);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line,  key,  uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          return uid;
        }
      }
    }
  }
  return "no UID found";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, username, pwd, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> pwd >> uid) {
        if (uid == Uid(pid)) {
          return username;
        }
      }
    }
  }
  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  long uptime = 0, starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int n = 0; n < 21; n++) {
      linestream >> value;
    }
    linestream >> starttime;
  starttime = starttime / sysconf(_SC_CLK_TCK);
  uptime =  UpTime() - starttime;
  return uptime;
  }
  return uptime;
}