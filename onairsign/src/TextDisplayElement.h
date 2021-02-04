#ifndef _TEXT_DISPLAY_ELEMENT_H_
#define _TEXT_DISPLAY_ELEMENT_H_

#define MAX_TEXT_LENGTH 15

#include "Logger.h"
#define USE_LOCAL_FONT 1
#include <MD_MAX72XX.h>

typedef struct {
    char c;
    int width;
} CharInfo_t;

// Not using hte DisplayElement superclass since this class is pushing the text directly to the matrix
class TextDisplayElement  {
    public:
        TextDisplayElement(MD_MAX72XX *matrix, const char *text, Logger *logger);
        bool Tick();
        void RefreshDisplay();

    private:
        Logger *logger;
        void BuildTextInfo(char *text, CharInfo_t *textInfo);
        MD_MAX72XX *matrix;
        int width;
        CharInfo_t text[MAX_TEXT_LENGTH]; // terminated by c=0
        unsigned int tickCounter = 0;
        
};

#endif