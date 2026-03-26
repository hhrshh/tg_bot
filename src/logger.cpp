#include "logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
std::string makeTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime {};
#ifdef _WIN32
    localtime_s(&localTime, &nowTime);
#else
    localtime_r(&nowTime, &localTime);
#endif

    std::ostringstream stream;
    stream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return stream.str();
}

const char* toLevelText(const LogLevel level) {
    switch (level) {
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARN";
        case LogLevel::Error:
            return "ERROR";
    }

    return "UNKNOWN";
}
} // namespace

void ILogger::info(const std::string& component, const std::string& message) {
    log(LogLevel::Info, component, message);
}

void ILogger::warn(const std::string& component, const std::string& message) {
    log(LogLevel::Warning, component, message);
}

void ILogger::error(const std::string& component, const std::string& message) {
    log(LogLevel::Error, component, message);
}

void ConsoleLogger::log(const LogLevel level, const std::string& component, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);

    std::ostream& output = (level == LogLevel::Error) ? std::cerr : std::cout;
    output << "[" << makeTimestamp() << "] "
           << "[" << toLevelText(level) << "] "
           << "[" << component << "] "
           << message
           << std::endl;
}
