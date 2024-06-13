#include "sound.h"

void initAudio() {
    ndspInit();
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
}

void exitAudio() {
    ndspExit();
}

bool loadWav(const char* filename, WAV* wav, void** audioBuffer) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return false;
    }

    fread(wav, 1, sizeof(WAV), file);

    // Check if the file is a valid WAV file
    if (memcmp(wav->riff, "RIFF", 4) != 0 || memcmp(wav->wave, "WAVE", 4) != 0) {
        printf("Invalid WAV file format\n");
        fclose(file);
        return false;
    }

    if (*audioBuffer) {
        linearFree(*audioBuffer);
    }

    *audioBuffer = linearAlloc(wav->subchunk2Size);
    if (!*audioBuffer) {
        printf("Failed to allocate audio buffer\n");
        fclose(file);
        return false;
    }

    fread(*audioBuffer, 1, wav->subchunk2Size, file);
    fclose(file);
    return true;
}

void playWav(WAV* wav, void* audioBuffer, ndspWaveBuf* waveBuf, int channel, bool loop) {
    ndspChnReset(channel);

    // Set the correct format based on the number of channels and bits per sample
    if (wav->numChannels == 2 && wav->bitsPerSample == 16) {
        ndspChnSetFormat(channel, NDSP_FORMAT_STEREO_PCM16);
    } else if (wav->numChannels == 1 && wav->bitsPerSample == 16) {
        ndspChnSetFormat(channel, NDSP_FORMAT_MONO_PCM16);
    } else {
        printf("Unsupported WAV format\n");
        return;
    }

    ndspChnSetRate(channel, wav->sampleRate);

    float mix[12];
    memset(mix, 0, sizeof(mix));
    mix[0] = 1.0;  // Left channel
    mix[1] = 1.0;  // Right channel
    ndspChnSetMix(channel, mix);

    memset(waveBuf, 0, sizeof(ndspWaveBuf));
    waveBuf->data_vaddr = audioBuffer;
    waveBuf->nsamples = wav->subchunk2Size / (wav->numChannels * (wav->bitsPerSample / 8));
    waveBuf->looping = loop;

    DSP_FlushDataCache(audioBuffer, wav->subchunk2Size);
    ndspChnWaveBufAdd(channel, waveBuf);
}
