#include "ConnectivityManager.h"

#define CONNECTIVITY_TIMEOUT_MS 2000

ConnectivityManager::ConnectivityManager(ConnectionStateUpdateCB_t cb) {
    this->connectionStateUpdateCB = cb;
}