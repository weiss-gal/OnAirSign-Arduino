#include "MessageHandler.h"
#include "Logger.h"
#include <Arduino.h>

#define TOKEN_SEPARATOR ' '
#define KEY_VALUE_SEPARATOR '='
#define TMP_BUF_LEN 128
#define CMD_HELLO "HELLO"
#define CMD_RESPONSE "RE"

#define ARG_CMDID "cmdid"

typedef struct {
    char *name;
    char *value;
} ArgPair_t;

MessageHandler::MessageHandler(Logger *logger){
    this->logger = logger;
}

// Making the token (everthing until the token) null terminated
// return pointer to the rest of the string
char *tokenizeUntil(char *inputStr, const char separator){
    int index = 0;
    while (inputStr[index] && inputStr[index] != separator)
        index++;

    if (!inputStr[index])
        return inputStr+index;

    inputStr[index] = 0;
    return inputStr+index+1;
}

char *parseNextArg(char *argsString, ArgPair_t & outPair){
    char *nextArg = tokenizeUntil(argsString, TOKEN_SEPARATOR);
    char *value = tokenizeUntil(argsString, KEY_VALUE_SEPARATOR);

    outPair.name = argsString;
    outPair.value = value;

    return nextArg;
}

bool MessageHandler::handleCommandHello(char *args,  char *response, int responseLen){
    logger->Log(LOG_LEVEL_INFO, "Handling command " CMD_HELLO " args: '%s'", args);
    
    char *cmdId = NULL; 
    
    char *argsPtr = args;
    while (*argsPtr){
        logger->Log(LOG_LEVEL_DEBUG, "Starting to parse arg %s", argsPtr);
        ArgPair_t pair;
        argsPtr = parseNextArg(argsPtr, pair);
        if (!strcmp(pair.name, ARG_CMDID))
            {
                if (*(pair.value))
                    cmdId = pair.value;
                else
                    logger->Log(LOG_LEVEL_WARNING, "Received empty command id", pair.name, pair.value);
                
            }
        else 
            logger->Log(LOG_LEVEL_WARNING, "Unknown argument: name='%s' value='%s'", pair.name, pair.value);
    }

    logger->Log(LOG_LEVEL_DEBUG, "Done parsing HELLO command, command id is %s", cmdId ? cmdId : "empty");
 
    bool responseBufferExceeded = false;
    int count = snprintf(response, responseLen, CMD_RESPONSE " " CMD_HELLO); 
    if (cmdId) 
        // Adding 1 for null termination
        if (count + snprintf(response + count, responseLen - count, " %s=%s", ARG_CMDID, cmdId) + 1 > responseLen)
            responseBufferExceeded = true;

    if (responseBufferExceeded) {
        logger->Log(LOG_LEVEL_ERROR, "Command: %s response buffer of length %d exceeded", CMD_HELLO, responseLen);
        return false;
    }
    return true;
}

bool MessageHandler::HandleMessage(const char *message, char *response, int responseLen){
    int messageLength = strlen(message);
    if (messageLength + 1> TMP_BUF_LEN) {
        logger->Log(LOG_LEVEL_ERROR, "Message Handler cannot handle a message of length %d", messageLength);
        return false;
    }

    char tmpBuf[TMP_BUF_LEN];
    strcpy(tmpBuf, message);
    
    char *argsString = tokenizeUntil(tmpBuf, TOKEN_SEPARATOR);

    
    if (!strcmp(tmpBuf, CMD_HELLO)) 
        return handleCommandHello(argsString, response, responseLen);
 
    logger->Log(LOG_LEVEL_WARNING, "Unknown command '%s'", tmpBuf);
    return false;
}