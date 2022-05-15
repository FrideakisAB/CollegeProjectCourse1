#include "Log.h"

#include <chrono>
#include <iomanip>

Log::Log()
{
    logFileStream.open("Log.txt", std::ios::trunc);
}

Log::~Log()
{
    logFileStream.close();
}

void Log::writeMessage(const std::string &severity, const std::string &msg)
{
    using clock = std::chrono::system_clock;
    std::time_t currentTime = clock::to_time_t(clock::now());

    tm localTime{};
    localtime_s(&localTime, &currentTime);

    std::stringstream resultString;
    resultString << "[" << severity << "]"
                 << std::put_time(&localTime, "[%F %T]: ")
                 << msg << std::endl;

    logFileStream << resultString.str();
}

void Log::Error(const std::string &msg)
{
    writeMessage("Error", msg);
}

void Log::Warning(const std::string &msg)
{
    writeMessage("Warning", msg);
}

void Log::Info(const std::string &msg)
{
    writeMessage("Info", msg);
}

void Log::Debug(const std::string &msg)
{
    writeMessage("Debug", msg);
}

Log &Log::Get()
{
    if (instance == nullptr)
        instance = new Log();

    return *instance;
}

void Log::Destroy()
{
    delete instance;
}
