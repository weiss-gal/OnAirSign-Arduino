#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_
#include "Logger.h"

// This is what would come from the host
typedef struct {
    bool isAudioPlaying;
    bool isAudioCapturing;
    bool isCameraCapturing;
} RequestedDisplayState_t;

typedef void (*RequestDisplayStateUpdate_t)(RequestedDisplayState_t requestedState);
typedef void (*MessageReceived_t)();

class MessageHandler {
    public: 
        MessageHandler(RequestDisplayStateUpdate_t requestUpdateCB, MessageReceived_t messageRecievedCB, Logger *logger);
        bool HandleMessage(const char *message, char *response, int responseLen);

    private:    
        Logger *logger;
        bool handleCommandHello(char *args, char *response, int responseLen);
        bool handleCommandSetDisplay(char *args,  char *response, int responseLen);
        RequestDisplayStateUpdate_t requestUpdateCB;
        MessageReceived_t messageRecievedCB;
};

#endif