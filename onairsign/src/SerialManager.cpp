#include "SerialManager.h"
#include "SerialProtocol.h"

#define RESPONSE_MESSAGE_LEN 128

SerialManager::SerialManager(HardwareSerial *serial, MessageHandler *messageHandler, Logger *logger){
    this->serial = serial;
    this->messageHandler = messageHandler;
    this->logger = logger;
    memset(this->messageBuffer, 0, MSG_BUFFER_LEN);
}

void SerialManager::ProcessInput(){
    if (serial->available() > 0)  {
        messageBuffer[messageIndex++] = serial->read();
        
        // Detect message termination
        int terminationLength = strlen(MESSAGE_TERMINATION);
        if (messageIndex >= terminationLength && !strcmp(MESSAGE_TERMINATION, messageBuffer + messageIndex - terminationLength))
        {
            messageBuffer[messageIndex - terminationLength] = 0;
            char responseBuf[RESPONSE_MESSAGE_LEN];
            if (messageHandler->HandleMessage(messageBuffer, responseBuf, RESPONSE_MESSAGE_LEN)) {
                serial->print(responseBuf);
                serial->print(MESSAGE_TERMINATION);
            }

            // reset message buffer
            memset(messageBuffer, 0, MSG_BUFFER_LEN); // TODO: this is not needed.
            messageIndex = 0;
        }

        // Stopping before the last char, so we know the buffer is null-terminated and can be logged safely
        if (messageIndex == MSG_BUFFER_LEN - 1) {
            messageIndex = 0;
            logger->Log(LOG_LEVEL_WARNING, "Message buffer overflow. buffer=%s", messageBuffer);
        }
    }

 }