#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_
#include "Logger.h"

typedef void (*SendResponse_t)(const char *response);

class MessageHandler {
    public: 
        MessageHandler(Logger *logger);
        void HandleMessage(const char *message, SendResponse_t responseCB);

    private:
        Logger *logger;
        void handleCommandHello(char *args, SendResponse_t responseCB);

};

#endif