#include "Logger.h"
#include "SerialProtocol.h"
#include "Arduino.h"    

#define LOG_BUFFER_SIZE 256
#define LOG_SEPARATOR " "

Logger::Logger(LogLevel_t minLogLevel){
    this->minLogLevel = minLogLevel;
}

void Logger::Log(LogLevel_t level, const char* format, ...){
    if (level < minLogLevel)
        return;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    Serial.print(LOG_COMMAND_PREFIX);
    Serial.print(Logger::LogLevel2String(level));
    Serial.print(LOG_SEPARATOR);
    Serial.print(buffer);
    Serial.print(MESSAGE_TERMINATION);
    if (level == LOG_LEVEL_FATAL)
      Serial.flush();
    va_end(args);
}