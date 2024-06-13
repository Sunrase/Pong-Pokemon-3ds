#ifndef SOUND_H
#define SOUND_H
#include "common.h"
#include <malloc.h>

void initAudio();
void exitAudio();
bool loadWav(const char* filename, WAV* wav, void** audioBuffer);
void playWav(WAV* wav, void* audioBuffer, ndspWaveBuf* waveBuf, int channel, bool loop);

#endif