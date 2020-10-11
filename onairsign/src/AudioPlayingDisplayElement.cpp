#include "AudioPlayingDisplayElement.h"
#include "DisplayBitmaps.h"
#include "DisplayBitmapsUtils.h"

const int AudioPlayingDisplayElement::GetWidth(){
    return AUDIO_PLAYING_WIDTH;
}

bool AudioPlayingDisplayElement::Tick(){
    tickCounter++;

    return isPlaying ? tickCounter % 2 == 0 : tickCounter % 10 == 0;
}

uint8_t *AudioPlayingDisplayElement::GetBitmap(){
    if (isPlaying) {
        switch (tickCounter % 10) {
            case 0: //fallthrough
            case 1: 
                DisplayBitmapUtils::DisplayBitmapCopy(outputBuffer, DisplayBitmaps::AudioPlayingBase, AUDIO_PLAYING_WIDTH);
                break;
            case 2: //fallthrough
            case 3: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioPlayingBase, DisplayBitmaps::AudioPlaying_overlay1, AUDIO_PLAYING_WIDTH);
                break;
            case 4:  //fallthrough
            case 5: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioPlayingBase, DisplayBitmaps::AudioPlaying_overlay2, AUDIO_PLAYING_WIDTH);
                break;
            case 6:  //fallthrough
            case 7: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioPlayingBase, DisplayBitmaps::AudioPlaying_overlay3, AUDIO_PLAYING_WIDTH);
                break;
            case 8:  //fallthrough
            case 9: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioPlayingBase, DisplayBitmaps::AudioPlaying_overlay4, AUDIO_PLAYING_WIDTH);
                break;

        }
    } else {
         DisplayBitmapUtils::DisplayBitmapCopy(outputBuffer, DisplayBitmaps::AudioPlayingBase, AUDIO_PLAYING_WIDTH);
    }

    return outputBuffer;
}

 void AudioPlayingDisplayElement::SetIsPlaying(bool isPlaying){
     this->isPlaying = isPlaying;
 }