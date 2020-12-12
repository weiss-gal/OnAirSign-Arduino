#include "AudioCapturingDisplayElement.h"
#include "DisplayBitmaps.h"
#include "DisplayBitmapsUtils.h"

const int AudioCapturingDisplayElement::GetWidth(){
    return AUDIO_CAPTURING_WIDTH;
}

bool AudioCapturingDisplayElement::Tick(){
    tickCounter = (tickCounter + 1) % 8;

    return isPlaying ? tickCounter % 2 == 0 : tickCounter == 0;
}

uint8_t *AudioCapturingDisplayElement::GetBitmap(){
    if (isPlaying) {
        switch (tickCounter) {
            case 0: //fallthrough
            case 1: 
                DisplayBitmapUtils::DisplayBitmapCopy(outputBuffer, DisplayBitmaps::AudioCapturing_Base, AUDIO_CAPTURING_WIDTH);
                break;
            case 2: //fallthrough
            case 3: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioCapturing_Base, DisplayBitmaps::AudioCapturing_Overlay1, AUDIO_CAPTURING_WIDTH);
                break;
            case 4:  //fallthrough
            case 5: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioCapturing_Base, DisplayBitmaps::AudioCapturing_Overlay2, AUDIO_CAPTURING_WIDTH);
                break;
            case 6:  //fallthrough
            case 7: 
                DisplayBitmapUtils::DisplayBitmapAdd(outputBuffer, DisplayBitmaps::AudioCapturing_Base, DisplayBitmaps::AudioCapturing_Overlay3, AUDIO_CAPTURING_WIDTH);
        }
    } else {
         DisplayBitmapUtils::DisplayBitmapCopy(outputBuffer, DisplayBitmaps::AudioCapturing_Base, AUDIO_CAPTURING_WIDTH);
    }

    return outputBuffer;
}

 void AudioCapturingDisplayElement::SetIsPlaying(bool isPlaying){
     this->isPlaying = isPlaying;
 }