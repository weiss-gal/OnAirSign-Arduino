#ifndef _LOGGER_H_
#define _LOGGER_H_

enum LogLevel_t {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

class Logger {
    public:
        Logger();
        void Log(LogLevel_t level, const char* format...);

    
};

#endif