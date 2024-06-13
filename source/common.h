#ifndef COMMON_H
#define COMMON_H

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <citro3d.h>
#include <citro2d.h>
#include <3ds/ndsp/ndsp.h>
#include <3ds/services/fs.h>

#define RACKET_SPEED 4.0f
#define ANGLE 60
#define FLAT_ANGLE 180
#define BALL_SIDE 25.0f
#define MAX_IMAGES 16
#define SCREEN_SIZE_WIDTH   240.0f
#define SCREEN_SIZE_LENGTH  400.0f
#define RACKET_WIDTH 25.0f
#define RACKET_LENGTH 75.0f
#define DEPTH 1.0f
#define SPEED_INCREMENT 0.1f
#define MIN_SPEED 1.0f
#define ADDED_SPEED 1.0f
#define MIDDLE_OF_RACKET 34.0f // In reality 75/2 = 37.5 but it tends to corrupt the racket sprite
#define RECTANGLE_WIDTH 35.0f
#define RECTANGLE_LENGTH 380.0f
#define BOTTOM_TEXT_X 40.0f
#define BOTTOM_TEXT_Y 240.0f/2.0f-33.0f
#define X_POINTER_POS 38.0f
#define X_LOGO 25.0f
#define Y_LOGO 55.0f
#define FONT_SIZE_X 0.5f
#define FONT_SIZE_Y 0.5f

typedef struct {
    u8 riff[4];
    u32 chunkSize;
    u8 wave[4];
    u8 fmt[4];
    u32 subchunk1Size;
    u16 audioFormat;
    u16 numChannels;
    u32 sampleRate;
    u32 byteRate;
    u16 blockAlign;
    u16 bitsPerSample;
    u8 data[4];
    u32 subchunk2Size;
} WAV;

extern C2D_Image images[MAX_IMAGES];
extern C2D_SpriteSheet spriteSheet;
extern C2D_Font font;
extern C2D_TextBuf g_dynamicBuf, g_dynamicBuf_2, g_staticBuf, g_pauseBuf, g_htp_pauseBuf, g_resumeBuf, g_YnotBuf, g_botMMBuf, g_playermodeBuf, g_playerwallBuf, g_unbeatableAIBuf;
extern C2D_Text g_dynamicText, g_dynamicText_2, g_staticText, g_pauseText, g_htp_pauseText, g_resumeText, g_YnotText, g_botMMText, g_playermodeText, g_playerwallText, g_unbeatableAIText;
extern WAV wav_background, wav_effect;
extern ndspWaveBuf waveBuf_background, waveBuf_effect;
extern void* audioBuffer_background;
extern void* audioBuffer_effect;

extern const char pause[];
extern const char pause_resume[];
extern const char Y_not[];
extern const char CGM[];
extern const char playermode[];
extern const char playerwall[];
extern const char unbeatableAI[];
extern const char Htp_pause[];
extern const char Bot_MM[];

#endif