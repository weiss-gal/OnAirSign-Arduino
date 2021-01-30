#include "DisplayManager.h"
#include "AudioPlayingDisplayElement.h"
#include "AudioCapturingDisplayElement.h"
#include "TextDisplayElement.h"
#include <Arduino.h>

#define LEFT_ALIGNMENT 2
#define SEPARATOR_WIDTH 4

#define AUDIO_PLAYING_ELEMENT_INDEX 0
#define AUDIO_CAPTURING_ELEMENT_INDEX 1


DisplayManager::DisplayManager(MD_MAX72XX *matrix, Logger *logger){
    this->matrix = matrix;
    this->logger = logger;
  
    this->matrix->begin();
    this->matrix->clear();

    elements[AUDIO_PLAYING_ELEMENT_INDEX] = new AudioPlayingDisplayElement();
    elements[AUDIO_CAPTURING_ELEMENT_INDEX] = new AudioCapturingDisplayElement();
    textDisplayElement = new TextDisplayElement(matrix, "Disconnected", this->logger);
}

void DisplayManager::SetDisplayState(DisplayState_t newState){
    if (newState.isConnected != displayState.isConnected)
        isClearRequired = true;

    displayState = newState;
    logger->Log(LOG_LEVEL_INFO, "Setting new state to: Connected=%d, Audio playing=%d, Audio capturing=%d, Camera capturing=%d",
    newState.isConnected, newState.isAudioPlaying, newState.isAudioCapturing, newState.isCameraCapturing);
    
    elements[AUDIO_PLAYING_ELEMENT_INDEX]->SetIsPlaying(newState.isAudioPlaying);
    elements[AUDIO_CAPTURING_ELEMENT_INDEX]->SetIsPlaying(newState.isAudioCapturing);
}

// TODO: add internal timing (do not rely on the cadence of RefreshDisplay)
void DisplayManager::RefreshDisplay(){
    if (isClearRequired) {
        matrix->clear();
        isClearRequired = false;
    }

    if (displayState.isConnected) {
        int currColumn = matrix->getColumnCount()-1-LEFT_ALIGNMENT; // Starting from the leftmost position
        for (int i=0; i < DISPLAY_ELEMENTS_NUM ; i++){
            if (elements[i]->Tick()){
                uint8_t *buffer = elements[i]->GetBitmap();
                matrix->setBuffer(currColumn, elements[i]->GetWidth(), buffer);
            }
            currColumn -= elements[i]->GetWidth() + SEPARATOR_WIDTH;// +1 for separator
        }
    } else {
     
        // Display scrolling message
        if (textDisplayElement->Tick())
            textDisplayElement->RefreshDisplay();
        
    }
}