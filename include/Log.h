#ifndef LOG_H
#define LOG_H

#include <fstream>

class Log {
private:
    static inline Log *instance = nullptr;
    std::ofstream logFileStream{};

    Log();
    ~Log();

    void writeMessage(const std::string &severity, const std::string &msg);

public:
    static Log &Get();
    static void Destroy();

    void Error(const std::string &msg);
    void Warning(const std::string &msg);
    void Info(const std::string &msg);
    void Debug(const std::string &msg);
};

#endif
