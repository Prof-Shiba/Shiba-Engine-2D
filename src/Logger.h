#pragma once
#include <string>

const std::string CONSOLE_COLOR_RED = "\x1B[31m";
const std::string CONSOLE_COLOR_GREEN = "\x1B[32m";
const std::string CONSOLE_RESET_COLOR = "\x1B[0m";

class Logger {
  public:
    static void Log(const std::string& message);
    static void Err(const std::string& message);
};
