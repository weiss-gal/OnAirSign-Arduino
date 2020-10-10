#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_
#include "Logger.h"

typedef void (*SendResponse_t)(const char *response);

class MessageHandler {
    public: 
        MessageHandler(Logger *logger);
        bool HandleMessage(const char *message, char *response, int responseLen);

    private:    
        Logger *logger;
        bool handleCommandHello(char *args, char *response, int responseLen);

};

#endif