#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "./Logger.h"

const std::string RED = "\x1B[31m";
const std::string GREEN = "\x1B[32m";

std::string get_formatted_time() {
  const auto now = std::chrono::system_clock::now();
  const auto current_time = std::chrono::system_clock::to_time_t(now);
  std::tm time = *std::localtime(&current_time);

  std::ostringstream oss;
  oss << std::put_time(&time, "%H:%M:%S");
    
  return oss.str();
}


void Logger::Log(const std::string& message) {
  std::cout << GREEN << "LOG: [" << get_formatted_time() << "] " << message << std::endl; 
}

void Logger::Err(const std::string& message) {
  std::cerr << RED << "ERROR: [" << get_formatted_time() << "] " << message << std::endl; 
}
