#pragma once
#include <string>

const std::string RED = "\x1B[31m";
const std::string GREEN = "\x1B[32m";

class Logger {
  public:
    static void Log(const std::string& message);
    static void Err(const std::string& message);
};
