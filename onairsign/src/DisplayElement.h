#ifndef _DISPLAY_ELEMENT_H_
#define _DISPLAY_ELEMENT_H_

#include <Arduino.h>

class DisplayElement {
    public:
        // Getting the pixel width of the element so they can be placed correctly, width is not expected to change
        // when the element changes, if that's the case, GetWidth should provide the maximum width
        virtual const int GetWidth() = 0; 
        
        // Signals the element that "tick" time passed and it should apply relevant transformations / animations
        // if needed
        // Returns: true if the element was changed, false otherwise.
        virtual bool Tick() = 0;
        
        // Gets the current element bitmap as a vector of size 'GetWidth()' of uint8, each represent the column pixels 
        // as a bitmap
        virtual uint8_t *GetBitmap() = 0;
};

#endif