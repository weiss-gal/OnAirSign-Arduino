#include "ConnectivityManager.h"

// Assuming 100ms frames, this is the number of "ticks" to determine that the host is not connected
#define CONNECTIVITY_TIMEOUT_TICKS 30

ConnectivityManager::ConnectivityManager(ConnectionStateUpdateCB_t cb, Logger *logger) {
    this->connectionStateUpdateCB = cb;
    this->logger = logger;
    this->ticketCounter = CONNECTIVITY_TIMEOUT_TICKS;
}

void ConnectivityManager::UpdateIsConnected(bool isConnected){
    if (this->isConnected != isConnected){
        this->isConnected = isConnected;
        this->connectionStateUpdateCB(isConnected);
    }
}

void ConnectivityManager::Tick(){
    ticketCounter = ticketCounter == 0 ? 0 : ticketCounter - 1;
    UpdateIsConnected(ticketCounter > 0);
}

void ConnectivityManager::Ping(){
    ticketCounter = CONNECTIVITY_TIMEOUT_TICKS;
    UpdateIsConnected(true);
}