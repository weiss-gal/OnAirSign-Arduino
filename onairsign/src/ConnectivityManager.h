#ifndef _CONNECTIVITY_MANAGER_H_
#define _CONNECTIVITY_MANAGER_H_

typedef void (*ConnectionStateUpdateCB_t)();

class ConnectivityManager 
{
    public:
        ConnectivityManager(ConnectionStateUpdateCB_t);

        // Tell the connectivity manager active connection was detected
        void Ping();

    private:
        ConnectionStateUpdateCB_t connectionStateUpdateCB;
};

#endif 