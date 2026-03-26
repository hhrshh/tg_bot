#pragma once

#include <mutex>
#include <string>

enum class LogLevel {
    Info,
    Warning,
    Error
};

class ILogger {
public:
    virtual ~ILogger() = default;

    void info(const std::string& component, const std::string& message);
    void warn(const std::string& component, const std::string& message);
    void error(const std::string& component, const std::string& message);

    virtual void log(LogLevel level, const std::string& component, const std::string& message) = 0;
};

class ConsoleLogger final : public ILogger {
public:
    void log(LogLevel level, const std::string& component, const std::string& message) override;

private:
    std::mutex mutex_;
};
