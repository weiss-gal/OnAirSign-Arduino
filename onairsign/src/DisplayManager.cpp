#include "DisplayManager.h"
#include "AudioPlayingDisplayElement.h"
#include <Arduino.h>

DisplayManager::DisplayManager(MD_MAX72XX *matrix, Logger *logger){
    this->matrix = matrix;
    this->logger = logger;
  
    this->matrix->begin();
    this->matrix->clear();

    //TODO: put real elements
    elements[0] = new AudioPlayingDisplayElement();
    elements[1] = new AudioPlayingDisplayElement();
    elements[2] = new AudioPlayingDisplayElement();
    
}

void DisplayManager::SetDisplayState(DisplayState_t newState){
    displayState = newState;
    logger->Log(LOG_LEVEL_INFO, "Setting new state to: Connected=%d, Audio playing=%d, Audio capturing=%d, Camera capturing=%d",
    newState.isConnected, newState.isAudioPlaying, newState.isAudioCapturing, newState.isCameraCapturing);
}

// TODO: add internal timing (do not rely on the cadence of RefreshDisplay)
void DisplayManager::RefreshDisplay(){
    logger->Log(LOG_LEVEL_DEBUG, "Refreshing display started");

    int currColumn = matrix->getColumnCount()-1; // Starting from the leftmost position
    for (int i=0; i < DISPLAY_ELEMENTS_NUM ; i++){
        if (elements[i]->Tick()){
            uint8_t *buffer = elements[i]->GetBitmap();
            matrix->setBuffer(currColumn, elements[i]->GetWidth(), buffer);
        }
        currColumn -= elements[i]->GetWidth() + 1;// +1 for separator
    }
}