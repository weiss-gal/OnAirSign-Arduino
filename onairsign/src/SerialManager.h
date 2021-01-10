#ifndef _SERIAL_MANAGER_H_
#define _SERIAL_MANAGER_H_
#include "Logger.h"
#include "MessageHandler.h"
#include <Arduino.h>

#define MSG_BUFFER_LEN 128

class SerialManager {
    public:
        SerialManager(HardwareSerial *serial, MessageHandler *messageHandler, Logger *logger);
        void ProcessInput();

    private:
        HardwareSerial *serial;
        MessageHandler *messageHandler;
        Logger *logger;
        char messageBuffer[MSG_BUFFER_LEN];
        int messageIndex = 0;
};

#endif 