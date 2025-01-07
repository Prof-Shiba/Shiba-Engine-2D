#pragma once
#include <string>
#include <vector>

enum LogType {
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
};

struct LogEntry {
  LogType type;
  std::string message;
};

const std::string CONSOLE_COLOR_RED = "\x1B[31m";
const std::string CONSOLE_COLOR_GREEN = "\x1B[32m";
const std::string CONSOLE_COLOR_YELLOW = "\x1B[33m";
const std::string CONSOLE_RESET_COLOR = "\x1B[0m";

class Logger {
  public:
    static std::vector<LogEntry> all_messages;
    static void Log(const std::string& message);
    static void Err(const std::string& message);
    static void Warn(const std::string& message);
};
