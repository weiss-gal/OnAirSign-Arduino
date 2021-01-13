#ifndef _CONNECTIVITY_MANAGER_H_
#define _CONNECTIVITY_MANAGER_H_

#include "Logger.h"

typedef void (*ConnectionStateUpdateCB_t)(bool isConnected);

class ConnectivityManager 
{
    public:
        ConnectivityManager(ConnectionStateUpdateCB_t, Logger *logger);

        // Tell the connectivity manager active connection was detected
        void Ping();
        // process one 
        void Tick();

    private:
        void UpdateIsConnected(bool isConnected);
        ConnectionStateUpdateCB_t connectionStateUpdateCB;
        Logger *logger;
        bool isConnected = false;
        bool pingReceived = false;
        int ticketCounter;
};

#endif 