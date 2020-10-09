#include "SerialManager.h"
#include "SerialProtocol.h"

SerialManager::SerialManager(HardwareSerial *serial, MessageHandler *messageHandler, Logger *logger){
    this->serial = serial;
    this->messageHandler = messageHandler;
    this->logger = logger;
    memset(this->messageBuffer, 0, MSG_BUFFER_LEN);
}

void SerialManager::callbackHandlerStub(const char *response){
    logger->Log(LOG_LEVEL_DEBUG, "Sending response '%s'", response);
}

void SerialManager::ProcessInput(){
    if (serial->available() > 0)  {
        messageBuffer[messageIndex++] = serial->read();
        
        // Detect message termination
        int terminationLength = strlen(MESSAGE_TERMINATION);
        if (messageIndex >= terminationLength && !strcmp(MESSAGE_TERMINATION, messageBuffer + messageIndex - terminationLength))
        {
            messageBuffer[messageIndex - terminationLength] = 0;
            messageHandler->HandleMessage(messageBuffer, NULL);

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