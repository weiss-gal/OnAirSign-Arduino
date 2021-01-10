#include "DisplayBitmaps.h"

const uint8_t DisplayBitmaps::AudioPlayingBase[AUDIO_PLAYING_WIDTH] =
{
    0b00011000,
    0b00111100,
    0x7E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00
};

const uint8_t DisplayBitmaps::AudioPlaying_overlay1[AUDIO_PLAYING_WIDTH] =
{
    0x00,
    0x00,
    0x00,
    0x00,
    0x18,
    0x00,
    0x00,
    0x00,
    0x00
};

const uint8_t DisplayBitmaps::AudioPlaying_overlay2[AUDIO_PLAYING_WIDTH] =
{
    0x00,
    0x00,
    0x00,
    0x00,
    0x18,
    0x24,
    0x18,
    0x00,
    0x00
};

const uint8_t DisplayBitmaps::AudioPlaying_overlay3[AUDIO_PLAYING_WIDTH] =
{
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x24,
    0x5A,
    0x24,
    0x18
};

const uint8_t DisplayBitmaps::AudioPlaying_overlay4[AUDIO_PLAYING_WIDTH] =
{
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x42,
    0x24,
    0x18
};

const uint8_t DisplayBitmaps::AudioCapturing_Base[AUDIO_CAPTURING_WIDTH] =
{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000100,
    0b01111010,
    0b01111010,
    0b00000100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000
};

const uint8_t DisplayBitmaps::AudioCapturing_Overlay3[AUDIO_CAPTURING_WIDTH] =
{
    0b00000000,
    0b00000000,
    0b00011000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00000000,
    0b00000000
};

const uint8_t DisplayBitmaps::AudioCapturing_Overlay2[AUDIO_CAPTURING_WIDTH] =
{
    0b00000000,
    0b00011000,
    0b00100100,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00100100,
    0b00011000,
    0b00000000
};

const uint8_t DisplayBitmaps::AudioCapturing_Overlay1[AUDIO_CAPTURING_WIDTH] =
{
    0b00111100,
    0b01000010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01000010,
    0b00111100
};