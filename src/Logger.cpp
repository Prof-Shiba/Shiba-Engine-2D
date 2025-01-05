#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "./Logger.h"

std::vector<LogEntry> Logger::all_messages;

std::string get_formatted_time() {
  const auto now = std::chrono::system_clock::now();
  const auto current_time = std::chrono::system_clock::to_time_t(now);
  std::tm time = *std::localtime(&current_time);

  std::ostringstream oss;
  oss << std::put_time(&time, "%r");
    
  return oss.str();
}

void Logger::Log(const std::string& message) {
  LogEntry log_entry;
  log_entry.type = LOG_INFO;
  log_entry.message = "LOG: [" + get_formatted_time() + "] " + message;
  all_messages.push_back(log_entry);

  std::cout << CONSOLE_COLOR_GREEN << log_entry.message << CONSOLE_RESET_COLOR << std::endl;
}

void Logger::Err(const std::string& message) {
  LogEntry log_entry;
  log_entry.type = LOG_ERROR;
  log_entry.message = "ERROR: [" + get_formatted_time() + "] " + message;
  all_messages.push_back(log_entry);

  std::cerr << CONSOLE_COLOR_RED << log_entry.message << CONSOLE_RESET_COLOR << std::endl; 
}

void Logger::Warn(const std::string& message) {
  LogEntry log_entry;
  log_entry.type = LOG_WARNING;
  log_entry.message = "WARNING: [" + get_formatted_time() + "] " + message;
  all_messages.push_back(log_entry);

  std::cout << CONSOLE_COLOR_YELLOW << log_entry.message << CONSOLE_RESET_COLOR << std::endl; 
}
