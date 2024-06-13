#include "common.h"
#include "Buffer.h"
#include "Graphics.h"
#include "games_modes.h"
#include "animations.h"
#include "menus.h"
#include "sound.h"

void StartGame(int selection, C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    if (selection == 0)
    {
        Play_with_another_player(top, bottom);
    }
    if (selection == 1)
    {
        Play_against_wall(top, bottom);
    }
    if (selection == 2)
    {
        Unbeatable_AI(top, bottom);
    }
}

void MainMenu(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    
    int selection = 0;
    float y_pointer = 65.0f;
    float x_pointer = X_POINTER_POS;
    float mov = -0.3f;

    CreateMainMenuBufferTop();

    if (loadWav("romfs:/sound/MainMenu.wav", &wav_background, &audioBuffer_background)) {
        playWav(&wav_background, audioBuffer_background, &waveBuf_background, 0, true);
    }

    //Main Loop
    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) {
            break; // Break in order to return to Hb Menu
        }
        if (kDown & KEY_DOWN) if (selection < 2) {
            selection += 1;
            if (loadWav("romfs:/sound/move.wav", &wav_effect, &audioBuffer_effect)) {
                playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1,false);
            }
        }
        if (kDown & KEY_UP) if (selection > 0) {
            selection -= 1;
            if (loadWav("romfs:/sound/move.wav", &wav_effect, &audioBuffer_effect)) {
                playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1,false);
            }
        }
        if (selection == 1) {
            y_pointer = 123.0f;
        } else if (selection == 2) {
            y_pointer = 173.0f;
        } else if (y_pointer != 48.0f) {
            y_pointer = 48.0f;
        }
        if (kDown & KEY_A) {
            DeleteMainMenuBuffer();
            if (loadWav("romfs:/sound/A_MAIN.wav", &wav_effect, &audioBuffer_effect)) {
                playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1,false);
            }
            if (loadWav("romfs:/sound/InGame.wav", &wav_background, &audioBuffer_background)) {
                playWav(&wav_background, audioBuffer_background, &waveBuf_background, 0,true);
            }
            StartGame(selection, top, bottom);
            if (loadWav("romfs:/sound/MainMenu.wav", &wav_background, &audioBuffer_background)) {
                playWav(&wav_background, audioBuffer_background, &waveBuf_background,0, true);
            }
            selection = 0;
        }

        PointerAnimation(&x_pointer, &mov);
        
        /*printf("\x1b[1;1H");
        printf("x: %f", x_pointer);*/

        /*printf("\x1b[6;1H");
        printf("%d", selection);*/
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(top);
            C2D_DrawImageAt(images[10], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Background
            C2D_DrawImageAt(images[12], 30.0f, 3.0f, DEPTH, NULL, 1.0f, 1.0f);
            C2D_DrawText(&g_staticText, C2D_AtBaseline | C2D_WithColor, 40.0f, 25.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawImageAt(images[13], 55.0f, 43.0f, DEPTH, NULL, 1.0f, 1.0f);
            C2D_DrawText(&g_playermodeText, C2D_AtBaseline | C2D_WithColor, 65.0f, 65.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawImageAt(images[14], 55.0f, 118.0f, DEPTH, NULL, 1.0f, 1.0f);
            C2D_DrawText(&g_playerwallText, C2D_AtBaseline | C2D_WithColor, 65.0f, 140.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawImageAt(images[13], 55.0f, 168.0f, DEPTH, NULL, 1.0f, 1.0f);
            C2D_DrawText(&g_unbeatableAIText, C2D_AtBaseline | C2D_WithColor, 65.0f, 190.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawImageAt(images[5], x_pointer, y_pointer, DEPTH, NULL, 1.0f, 1.0f); // Pointer
            C2D_SceneBegin(bottom);
            C2D_DrawImageAt(images[11], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Background
            C2D_DrawImageAt(images[15], 15.0f, SCREEN_SIZE_WIDTH/2.0f-7.0f, DEPTH, NULL, 1.0f, 1.0f);
            C2D_DrawText(&g_botMMText, C2D_AtBaseline | C2D_WithColor, 25.0f, SCREEN_SIZE_WIDTH/2.0f+15.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }
        C3D_FrameEnd(0);

        gfxFlushBuffers();
    }
    C2D_TextBufDelete(g_botMMBuf);
    C2D_TextBufDelete(g_staticBuf);
}

void TitleScreen(C3D_RenderTarget* top, C3D_RenderTarget* bottom, float *y_logo, float *mov_logo, int *frame_count) {

    // Move logo
    LogoAnimation(y_logo, mov_logo);

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    {
        C2D_SceneBegin(top);
        C2D_DrawImageAt(images[6], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Background
        C2D_DrawImageAt(images[7], X_LOGO, *y_logo, DEPTH, NULL, 1.0f, 1.0f); // Logo
        C2D_DrawImageAt(images[9], SCREEN_SIZE_LENGTH-110.0f, SCREEN_SIZE_WIDTH-20.0f, DEPTH, NULL, 1.0f, 1.0f); // Author
        if (*frame_count <= 30) C2D_DrawImageAt(images[8], 150.0f, SCREEN_SIZE_WIDTH-23.0f, DEPTH, NULL, 1.0f, 1.0f); // Press A
        else if (*frame_count >= 60) *frame_count = 0;
        C2D_SceneBegin(bottom);
        C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
        C2D_DrawText(&g_botMMText, C2D_AtBaseline | C2D_WithColor, 25.0f, SCREEN_SIZE_WIDTH/2.0f+15.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
    }
    C3D_FrameEnd(0);

    *frame_count += 1;
}