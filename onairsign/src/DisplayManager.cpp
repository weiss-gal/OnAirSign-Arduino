#include "DisplayManager.h"
#include "AudioPlayingDisplayElement.h"
#include <Arduino.h>

DisplayManager::DisplayManager(MD_MAX72XX *matrix, Logger *logger){
    this->matrix = matrix;
    this->logger = logger;
  
    this->matrix->begin();
    this->matrix->clear();

    //TODO: put real elements
    audioPlaying = new AudioPlayingDisplayElement();
    audioCapturing = new AudioPlayingDisplayElement();
    cameraCapturing = new AudioPlayingDisplayElement();

    elements[0] = audioPlaying;
    elements[1] = audioCapturing;
    elements[2] = cameraCapturing;
}

void DisplayManager::SetDisplayState(DisplayState_t newState){
    displayState = newState;
    logger->Log(LOG_LEVEL_INFO, "Setting new state to: Connected=%d, Audio playing=%d, Audio capturing=%d, Camera capturing=%d",
    newState.isConnected, newState.isAudioPlaying, newState.isAudioCapturing, newState.isCameraCapturing);
    
    audioPlaying->SetIsPlaying(newState.isAudioPlaying);
    audioCapturing->SetIsPlaying(newState.isAudioCapturing);
    cameraCapturing->SetIsPlaying(newState.isCameraCapturing);
}

// TODO: add internal timing (do not rely on the cadence of RefreshDisplay)
void DisplayManager::RefreshDisplay(){

    int currColumn = matrix->getColumnCount()-1; // Starting from the leftmost position
    for (int i=0; i < DISPLAY_ELEMENTS_NUM ; i++){
        if (elements[i]->Tick()){
            uint8_t *buffer = elements[i]->GetBitmap();
            matrix->setBuffer(currColumn, elements[i]->GetWidth(), buffer);
        }
        currColumn -= elements[i]->GetWidth() + 1;// +1 for separator
    }
}