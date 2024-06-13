#include "common.h"
#include "Buffer.h"
#include "Graphics.h"
#include "menus.h"
#include "sound.h"

int main(int argc, char **argv) {

    // Target render for top and bottom screen
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Initialisation
    initGraphics();

    // Load Images
    loadImages(spriteSheet, images, MAX_IMAGES, font);

    // Load Font
    LoadFont(&font);

    // Init Audio
    initAudio();

    float y_logo = Y_LOGO;
    float mov_logo = -0.2f;
    int frame_count = 0;

    CreateMainMenuBufferBottom();

    if (!loadWav("romfs:/sound/A_MAIN.wav", &wav_effect, &audioBuffer_effect)) {
        exitAudio();
        exitGraphics();
    }

    if (loadWav("romfs:/sound/Title.wav", &wav_background, &audioBuffer_background)) {
        playWav(&wav_background, audioBuffer_background, &waveBuf_background,0,true);
    }

    //Main Loop
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // Break in order to return to Hb Menu
        TitleScreen(top, bottom, &y_logo, &mov_logo, &frame_count);
        if (kDown & KEY_A) {
            playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1,false);
            MainMenu(top, bottom);
            break;
        }

        gfxFlushBuffers();

    }

    exitAudio();

    exitGraphics();

    return 0;
}

/*To do :   nothing for the moment
*/