#ifndef _DISPLAY_BITMATPS_H_
#define _DISPLAY_BITMATPS_H_
#include "Arduino.h"

#define AUDIO_PLAYING_WIDTH 9
#define AUDIO_CAPTURING_WIDTH 12

class DisplayBitmaps {
  public: 
    static const uint8_t AudioPlayingBase[];
    
    static const uint8_t AudioPlaying_overlay1[];
    
    static const uint8_t AudioPlaying_overlay2[];
    
    static const uint8_t AudioPlaying_overlay3[];
    
    static const uint8_t AudioPlaying_overlay4[];

    static const uint8_t AudioCapturing_Base[];

    static const uint8_t AudioCapturing_Overlay1[];
    
    static const uint8_t AudioCapturing_Overlay2[];
    
    static const uint8_t AudioCapturing_Overlay3[];
};
#endif