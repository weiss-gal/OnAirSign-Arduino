#include "TextDisplayElement.h"
#include "Logger.h"

#define ANIMATION_SPEED_TICKS 2
#define SPACE_WIDTH 1 // in columns

void TextDisplayElement::BuildTextInfo(char *text, CharInfo_t *textInfo) {
    int count=0;
    uint8_t dummyBuffer[12];
    for (; *text && count < MAX_TEXT_LENGTH; text++, count++) {
        textInfo[count].c = *text;
        int width = matrix->getChar(*text, sizeof(dummyBuffer), dummyBuffer);
        if (!width)
            logger->Log(LOG_LEVEL_FATAL, "Failed to get char properties for char '%c'", *text);
        textInfo[count].width = width;
    }

    // add termination
    textInfo[count].c = 0;

}

TextDisplayElement::TextDisplayElement(MD_MAX72XX *matrix, const char *text, Logger *logger){
    this->logger = logger;
    this->matrix = matrix;
    BuildTextInfo((char *)text, this->text);
    // figure text length 
    int length = 0;
    for (int i = 0; this->text[i].c; i++)
        length += this->text[i].width + SPACE_WIDTH ;
    
    // remove redundant spacing, add screen width
    this->width = length - SPACE_WIDTH + matrix->getColumnCount();
    
}

bool TextDisplayElement::Tick(){
    return ++tickCounter % ANIMATION_SPEED_TICKS == 0;
}

void TextDisplayElement::RefreshDisplay(){
    matrix->clear();
    int matrixWidth = matrix->getColumnCount();
    int startPoint = (tickCounter / ANIMATION_SPEED_TICKS) % this->width;
    int currCol = startPoint;
    CharInfo_t *textPtr = text;

    while (currCol >= 0 && textPtr->c){
        // check if part of the char is within the viewable part
        if ((currCol - textPtr->width) < matrixWidth) {
            matrix->setChar(currCol, textPtr->c);
        }

        currCol -= textPtr->width + SPACE_WIDTH;
        textPtr++;
    }
}
