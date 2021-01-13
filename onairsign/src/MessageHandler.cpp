#include "MessageHandler.h"
#include "Logger.h"
#include "DisplayManager.h"
#include "SerialProtocol.h"
#include <Arduino.h>

#define TOKEN_SEPARATOR ' '
#define KEY_VALUE_SEPARATOR '='
#define TMP_BUF_LEN 128
#define CMD_HELLO "HELLO"
#define CMD_SET_DISPLAY "SET_DISPLAY"
#define CMD_RESPONSE "RE"

#define ARG_CMDID "cmdid"
#define ARG_STATE "state"
#define ARG_STATUS "status"
#define ARG_REASON "reason"

MessageHandler::MessageHandler(RequestDisplayStateUpdate_t requestUpdateCB, MessageReceived_t messageRecievedCB, Logger *logger){
    this->requestUpdateCB = requestUpdateCB;
    this->logger = logger;
    this->messageRecievedCB = messageRecievedCB;
}

typedef struct {
    char *name;
    char *value;
} ArgPair_t;

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

// TODO: implement version in response, consider using build time
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
    if (cmdId) //TODO: put cmdid=0 if its missing
        // Adding 1 for null termination
        if (count + snprintf(response + count, responseLen - count, " %s=%s", ARG_CMDID, cmdId) + 1 > responseLen)
            responseBufferExceeded = true;

    if (responseBufferExceeded) {
        logger->Log(LOG_LEVEL_ERROR, "Command: %s response buffer of length %d exceeded", CMD_HELLO, responseLen);
        return false;
    }

    // Hello is used as a keepalive
    messageRecievedCB();

    return true;
}

static bool stateStr2DisplayState(const char *stateStr, RequestedDisplayState_t &displayState){
    int index = 0;
    while (stateStr[index] == '0' || stateStr[index] == '1')
        index++;

    if (index != 3 || stateStr[index] != 0)
        return false;
    
    displayState.isAudioPlaying = stateStr[0] == '1';
    displayState.isAudioCapturing = stateStr[1] == '1';
    displayState.isCameraCapturing = stateStr[2] == '1'; 
 
    return true;    
}

bool MessageHandler::handleCommandSetDisplay(char *args,  char *response, int responseLen){
    logger->Log(LOG_LEVEL_INFO, "Handling command " CMD_SET_DISPLAY " args: '%s'", args);
    
    char *cmdId = NULL; 
    bool stateReceived = false;
    RequestedDisplayState_t requestedDisplayState;
    int reasonCode = REASON_CODE_OK;
    
    char *argsPtr = args;
    while (reasonCode == REASON_CODE_OK && *argsPtr){
        logger->Log(LOG_LEVEL_DEBUG, "Starting to parse arg %s", argsPtr);
        ArgPair_t pair;
        argsPtr = parseNextArg(argsPtr, pair);
        if (!strcmp(pair.name, ARG_CMDID)) {
            if (*(pair.value))
                cmdId = pair.value;
            else
                logger->Log(LOG_LEVEL_WARNING, "Received empty command id", pair.name, pair.value);
        } else if (!strcmp(pair.name, ARG_STATE)) {
            if (!*(pair.value) || !stateStr2DisplayState(pair.value, requestedDisplayState)) {
                reasonCode = REASSON_CODE_INVALID_OPTION;
                logger->Log(LOG_LEVEL_WARNING, "Invalid %s argument: '%s'", ARG_STATE, pair.value);
            } else 
                stateReceived = true;
        } else 
            logger->Log(LOG_LEVEL_WARNING, "Unknown argument: name='%s' value='%s'", pair.name, pair.value);
    }

    logger->Log(LOG_LEVEL_DEBUG, "Done parsing %s command, command id is %s, ",
     CMD_SET_DISPLAY, cmdId ? cmdId : "empty");

    // If this is a valid command, request to set displayState
    if (reasonCode == REASON_CODE_OK && !stateReceived)
        reasonCode = REASON_CODE_MISSING_MANDATORY_OPTION;

    if (reasonCode == REASON_CODE_OK){
        requestUpdateCB(requestedDisplayState);
    }  
 
    bool responseBufferExceeded = false;
    int count = snprintf(response, responseLen, CMD_RESPONSE " " CMD_SET_DISPLAY " " ARG_CMDID "=%s " ARG_STATUS "=%s",
        cmdId ? cmdId : "0", reasonCode == REASON_CODE_OK ? "ok" : "nok"); 

    if (reasonCode != REASON_CODE_OK) 
        // Adding 1 for null termination
        if (count + snprintf(response + count, responseLen - count, " %s=%d", ARG_REASON, reasonCode) + 1 > responseLen)
            responseBufferExceeded = true;

    if (responseBufferExceeded) {
        logger->Log(LOG_LEVEL_ERROR, "Command: %s response buffer of length %d exceeded", CMD_HELLO, responseLen);
        return false;
    }

    // SET_STATE is also used as a keepalive
    messageRecievedCB();
  
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
    else if (!strcmp(tmpBuf, CMD_SET_DISPLAY))
        return handleCommandSetDisplay(argsString, response, responseLen);
 
    logger->Log(LOG_LEVEL_WARNING, "Unknown command '%s'", tmpBuf);
    return false;
}