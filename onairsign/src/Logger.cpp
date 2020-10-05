#include "Logger.h"
#include "SerialProtocol.h"
#include "Arduino.h"    

#define LOG_BUFFER_SIZE 256

Logger::Logger(){

}

void Logger::Log(LogLevel_t level, const char* format...){
    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    
    vsnprintf(buffer, LOG_BUFFER_SIZE, format, args);
    Serial.print(buffer);
    Serial.print(MESSAGE_TERMINATION);


    va_end(args);
}