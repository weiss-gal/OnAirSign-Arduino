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
        void Log(LogLevel_t level, const char *format, ...)  
            __attribute__ ((format (printf, 3, 4))); // 3,4 instead of 2,3 because this is a class method.
        
    private:
    static const char *LogLevel2String(LogLevel_t level) {
        switch (level) {
            case LOG_LEVEL_DEBUG: return "DEBUG";
            case LOG_LEVEL_INFO: return "INFO";
            case LOG_LEVEL_WARNING: return "WARNING";
            case LOG_LEVEL_ERROR: return "ERROR";
        }

        return "UNKNOWN";
    }

};

#endif