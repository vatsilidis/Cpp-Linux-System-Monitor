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
  string line, key, value;
  std::ifstream myStream(kOSPath);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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
  string line, key;
  float value;
  float memTotal;
  float memFree;
  std::ifstream myStream(kProcDirectory + kMeminfoFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
	  std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value * 0.001;
        } else if (key == "MemFree:") {
          memFree = value * 0.001;
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
  string line, value;
  long utime, stime, cutime, cstime;
  std::ifstream myStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (myStream.is_open()) {
    std::getline(myStream, line);
    std::istringstream linestream(line);
    for (int n = 0; n < 13; n++) {
      linestream >> value;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line, key;
  long user, nice, system, idle, iowait, irq, softirq, steal, active;
  std::ifstream myStream(kProcDirectory + kStatFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >>
             irq >> softirq >> steal) {
        if (key == "cpu") {
          active = user + nice + system + irq + softirq + steal;
        }
      }
    }
  }
  return active / sysconf(_SC_CLK_TCK);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {  
  string line, key;
  long user, nice, system, idle, iowait, irq, softirq, steal, idleTime;
  std::ifstream myStream(kProcDirectory + kStatFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >>
             irq >> softirq >> steal) {
        if (key == "cpu") {
          idleTime = idle + iowait;
        }
      }
    }
  }
  return idleTime / sysconf(_SC_CLK_TCK);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key, value;
  vector<string> values{};
  std::ifstream myStream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          linestream >> value;
          values.push_back(value);
        }
      }
    }
  }
  return values;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key;
  int total_proc;
  std::ifstream myStream(kProcDirectory + kStatFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> total_proc) {
        if (key == "processes") {
          return total_proc;
        }
      }
    }
  }
  return total_proc;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int runnig_proc;
  std::ifstream myStream(kProcDirectory + kStatFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> runnig_proc) {
        if (key == "procs_running") {
          return runnig_proc;
        }
      }
    }
  }
  return runnig_proc;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line,  key,  uid;
  std::ifstream myStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          return uid;
        }
      }
    }
  }
  return uid;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream myStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (myStream.is_open()) {
    std::getline(myStream, line);
  }
  return line;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, username, pwd, uid;
  std::ifstream myStream(kPasswordPath);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
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

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key;
  float value;
  int ram;
  std::ifstream myStream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (myStream.is_open()) {
    while (std::getline(myStream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          ram = value * 0.001;
        }
      }
    }
  }
  return to_string(ram);
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  long uptime;
  std::ifstream myStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (myStream.is_open()) {
    std::getline(myStream, line);
    std::istringstream linestream(line);
    for (int n = 0; n < 21; n++) {
      linestream >> value;
    }
    linestream >> uptime;
  }
  return uptime / sysconf(_SC_CLK_TCK);
}
