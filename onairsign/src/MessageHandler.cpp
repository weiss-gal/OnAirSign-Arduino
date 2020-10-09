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


void MessageHandler::handleCommandHello(char *args, SendResponse_t responseCB){
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
 
    char responseBuf[TMP_BUF_LEN];
    int count = snprintf(responseBuf, TMP_BUF_LEN, CMD_RESPONSE " " CMD_HELLO); 
    if (cmdId) 
        snprintf(responseBuf + count, TMP_BUF_LEN - count, ARG_CMDID "=%s", cmdId);

    responseBuf[TMP_BUF_LEN - 1] = 0;

    //responseCB(responseBuf);
}

void MessageHandler::HandleMessage(const char *message, SendResponse_t responseCB){
    int messageLength = strlen(message);
    if (messageLength + 1> TMP_BUF_LEN) {
        logger->Log(LOG_LEVEL_ERROR, "Message Handler cannot handle a message of length %d", messageLength);
        return;
    }

    char tmpBuf[TMP_BUF_LEN];
    strcpy(tmpBuf, message);
    
    char *argsString = tokenizeUntil(tmpBuf, TOKEN_SEPARATOR);

    if (!strcmp(tmpBuf, CMD_HELLO)) {
        handleCommandHello(argsString, responseCB);
    } else {
        logger->Log(LOG_LEVEL_WARNING, "Unknown command '%s'", tmpBuf);
    }
}