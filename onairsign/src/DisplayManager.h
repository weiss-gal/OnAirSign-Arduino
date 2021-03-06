#ifndef _DISPLAY_MANAGER_H_
#define _DISPLAY_MANAGER_H_

#include "Logger.h"
#include "DisplayElement.h"
#include "TextDisplayElement.h"
#include "AudioPlayingDisplayElement.h"
#include "AudioCapturingDisplayElement.h"
#include <MD_MAX72XX.h>

#define DISPLAY_ELEMENTS_NUM 2

typedef struct {
    bool isConnected;
    bool isAudioPlaying;
    bool isAudioCapturing;
    bool isCameraCapturing;
} DisplayState_t;

class DisplayManager {
    public:
        DisplayManager(MD_MAX72XX *matrix, Logger *logger);
        void SetDisplayState(DisplayState_t newState);
        void RefreshDisplay();
        
    private:
        MD_MAX72XX *matrix;
        Logger *logger;
        DisplayState_t displayState = {false, false, false, false};
        DisplayElement *elements[DISPLAY_ELEMENTS_NUM];
        TextDisplayElement *textDisplayElement;
        bool isClearRequired;
};

#endif