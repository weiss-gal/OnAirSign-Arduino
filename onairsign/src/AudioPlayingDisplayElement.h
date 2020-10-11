#ifndef _AUDIO_PLAYING_DISPLAY_ELEMENT_H_
#define _AUDIO_PLAYING_DISPLAY_ELEMENT_H_

#include "DisplayElement.h"
#include "DisplayBitmaps.h"

class AudioPlayingDisplayElement : public DisplayElement {
    public:
        const int GetWidth(); 
        bool Tick();
        uint8_t *GetBitmap();
        void SetIsPlaying(bool isPlaying);
    
    private:
        int tickCounter = 0;
        bool isPlaying = false;
        // TODO: this is not really the way to do it, reusing the buffer make the code
        // not-reentrant (and definetely not thread safe)
        // but its fast and easy. 
        uint8_t outputBuffer[AUDIO_PLAYING_WIDTH]; 
};

#endif