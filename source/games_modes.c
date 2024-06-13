#include "common.h"
#include "Buffer.h"
#include "Graphics.h"
#include "Ball.h"
#include "Pause.h"
#include "relative_to_game.h"
#include "sound.h"

void Play_with_another_player(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {

    if (!loadWav("romfs:/sound/Bond.wav", &wav_effect, &audioBuffer_effect)) {
        exitGraphics();
        exitAudio();
    }
    
    bool collision = false;
    float speed[2];   // Movement speed
    float x;
    float y;
    float y_racket_1 = SCREEN_SIZE_WIDTH/2.0f - MIDDLE_OF_RACKET;
    float y_racket_2 = SCREEN_SIZE_WIDTH/2.0f - MIDDLE_OF_RACKET;
    int score_player_1 = 0;
    int score_player_2 = 0;

    // Text buffer
    g_dynamicBuf = C2D_TextBufNew(3);
    g_dynamicBuf_2 = C2D_TextBufNew(3);
    CreateBuffer(&g_htp_pauseBuf, &g_htp_pauseText, Htp_pause, font);

    ballInit(&x, &y, speed);

    // Boucle principale
    while (aptMainLoop()) {

        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        // Moving rackets
        if (kHeld & KEY_DOWN) {
            if (y_racket_1 < SCREEN_SIZE_WIDTH - RACKET_LENGTH) {
                    y_racket_1 += RACKET_SPEED;
                }
        }    
        if (kHeld & KEY_UP) {
            if (y_racket_1 > 0.0f) {
                    y_racket_1 -= RACKET_SPEED;
                }
        }
        if (kHeld & KEY_B) {
            if (y_racket_2 < SCREEN_SIZE_WIDTH - RACKET_LENGTH) {
                    y_racket_2 += RACKET_SPEED;
                }
        }
        if (kHeld & KEY_X) {
            if (y_racket_2 > 0.0f) {
                    y_racket_2 -= RACKET_SPEED;
                }
        }
        if (kDown & KEY_SELECT) {
            Pause(top, bottom);
        }
        if (kDown & KEY_Y) {
            break; // Break in order to return to the main loop
        }

        Movement(&x, &y, speed);

        // Detect collisions and reverse y axe
        if (y <= 0.0f || y >= SCREEN_SIZE_WIDTH - BALL_SIDE) speed[1] = -speed[1], collision = true;

        // Collision left racket
        if (x <= RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_1) & (y + (BALL_SIDE/2.0f) <= y_racket_1 + RACKET_LENGTH)) {
                speed[0] = -speed[0];
                collision = true;
            } else {
                score_player_1 += 1;
                ballInit(&x, &y, speed);
            }
        }

        // Collision right racket
        if (x + RACKET_WIDTH >= SCREEN_SIZE_LENGTH - RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_2) & (y + (BALL_SIDE/2.0f) <= y_racket_2 + RACKET_LENGTH)) {
                speed[0] = -speed[0];
                collision = true;
            } else {
                score_player_2 += 1;
                ballInit(&x, &y, speed);
            }
        }
        
        // Increase speed when collision
        ImpSpeedCol(collision, speed);

        UpdateScoreText(score_player_1, score_player_2); // Score update

        // Debugging, also enable consoleInit(GFX_BOTTOM, NULL) in intGraphics()
        /*printf("\x1b[1;1H");
        printf("x: %f y: %f", x, y);
        printf("\x1b[2;1H");
        printf("speed x : %f", speed[0]);
        printf("\x1b[3;1H");
        printf("speed y : %f", speed[1]);
        printf("\x1b[4;1H");
        printf("y_racket_1 : %f", y_racket_1);
        printf("\x1b[5;1H");
        printf("y_racket_1 : %f", y_racket_2);*/

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            // Starting render on top screen
            C2D_SceneBegin(top);

            // Draw images
            C2D_DrawImageAt(images[0], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Map
            C2D_DrawText(&g_dynamicText, C2D_AtBaseline | C2D_WithColor, 220.0f, 40.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF)); // Scores
            C2D_DrawText(&g_dynamicText_2, C2D_AtBaseline | C2D_WithColor, 166.0f, 40.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF));
            C2D_DrawImageAt(images[1], 0.0f, y_racket_1, DEPTH, NULL, 1.0f, 1.0f); // Left racket
            C2D_DrawImageAt(images[2], SCREEN_SIZE_LENGTH - RACKET_WIDTH, y_racket_2, DEPTH, NULL, 1.0f, 1.0f); // Right racket
            C2D_DrawImageAt(images[3], x, y, DEPTH, NULL, 1.0f, 1.0f); // Ball

            // Bottom screen
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_htp_pauseText, C2D_AtBaseline | C2D_WithColor, BOTTOM_TEXT_X, BOTTOM_TEXT_Y, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }

        // End of render
        C3D_FrameEnd(0);

        gfxFlushBuffers();

        if (collision) {
            collision = false;
            playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
        }
    }
    C2D_TextBufDelete(g_dynamicBuf);
    C2D_TextBufDelete(g_dynamicBuf_2);
    C2D_TextBufDelete(g_htp_pauseBuf);
    CreateMainMenuBufferTop();
    CreateMainMenuBufferBottom();
    if (loadWav("romfs:/sound/Back.wav", &wav_effect, &audioBuffer_effect)) {
        playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
    }
}

void Unbeatable_AI(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {

    if (!loadWav("romfs:/sound/Bond.wav", &wav_effect, &audioBuffer_effect)) {
        exitGraphics();
        exitAudio();
    }
    
    bool collision = false;
    float speed[2];   // Movement speed
    float x;
    float y;
    float y_racket_1 = SCREEN_SIZE_WIDTH/2.0f - MIDDLE_OF_RACKET;
    float y_racket_2 = SCREEN_SIZE_WIDTH/2.0f - MIDDLE_OF_RACKET;
    int score_player_1 = 0;
    int score_player_2 = 0;

    // Text buffer
    g_dynamicBuf = C2D_TextBufNew(3);
    g_dynamicBuf_2 = C2D_TextBufNew(3);
    CreateBuffer(&g_htp_pauseBuf, &g_htp_pauseText, Htp_pause, font);

    ballInit(&x, &y, speed);

    // Boucle principale
    while (aptMainLoop()) {

        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        // Moving rackets
        if (kHeld & KEY_DOWN) {
            if (y_racket_1 < SCREEN_SIZE_WIDTH - RACKET_LENGTH) {
                    y_racket_1 += RACKET_SPEED;
                }
        }    
        if (kHeld & KEY_UP) {
            if (y_racket_1 > 0.0f) {
                    y_racket_1 -= RACKET_SPEED;
                }
        }
        if (kDown & KEY_SELECT) {
            Pause(top, bottom);
        }
        if (kDown & KEY_Y) {
            break; // Break in order to return to the main loop
        }
        if ((x > SCREEN_SIZE_LENGTH/4.0f) & (speed[0] > 0.0f)) {
            if ((y < y_racket_2+BALL_SIDE) & (y_racket_2 > 0.0f)) y_racket_2 -= RACKET_SPEED;
            if ((y > y_racket_2+BALL_SIDE) & (y_racket_2 + RACKET_LENGTH < SCREEN_SIZE_WIDTH)) y_racket_2 += RACKET_SPEED;
        }

        Movement(&x, &y, speed);

        // Detect collisions and reverse y axe
        if (y <= 0.0f || y >= SCREEN_SIZE_WIDTH - BALL_SIDE) speed[1] = -speed[1], collision = true;

        // Collision left racket
        if (x <= RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_1) & (y + (BALL_SIDE/2.0f) <= y_racket_1 + RACKET_LENGTH)) {
                speed[0] = -speed[0];
                collision = true;
            } else {
                score_player_1 += 1;
                ballInit(&x, &y, speed);
            }
        }

        // Collision right racket
        if (x + RACKET_WIDTH >= SCREEN_SIZE_LENGTH - RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_2) & (y + (BALL_SIDE/2.0f) <= y_racket_2 + RACKET_LENGTH)) {
                speed[0] = -speed[0];
                collision = true;
            } else {
                score_player_2 += 1;
                ballInit(&x, &y, speed);
            }
        }
        
        // Increase speed when collision
        ImpSpeedCol(collision, speed);

        UpdateScoreText(score_player_1, score_player_2); // Score update

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            // Starting render on top screen
            C2D_SceneBegin(top);

            // Draw images
            C2D_DrawImageAt(images[0], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Map
            C2D_DrawText(&g_dynamicText, C2D_AtBaseline | C2D_WithColor, 220.0f, 40.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF)); // Scores
            C2D_DrawText(&g_dynamicText_2, C2D_AtBaseline | C2D_WithColor, 166.0f, 40.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF));
            C2D_DrawImageAt(images[1], 0.0f, y_racket_1, DEPTH, NULL, 1.0f, 1.0f); // Left racket
            C2D_DrawImageAt(images[2], SCREEN_SIZE_LENGTH - RACKET_WIDTH, y_racket_2, DEPTH, NULL, 1.0f, 1.0f); // Right racket
            C2D_DrawImageAt(images[3], x, y, DEPTH, NULL, 1.0f, 1.0f); // Ball

            // Bottom screen
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_htp_pauseText, C2D_AtBaseline | C2D_WithColor, BOTTOM_TEXT_X, BOTTOM_TEXT_Y, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }

        // End of render
        C3D_FrameEnd(0);

        gfxFlushBuffers();

        if (collision) {
            collision = false;
            playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
        }
    }
    C2D_TextBufDelete(g_dynamicBuf);
    C2D_TextBufDelete(g_dynamicBuf_2);
    C2D_TextBufDelete(g_htp_pauseBuf);
    CreateMainMenuBufferTop();
    CreateMainMenuBufferBottom();
    if (loadWav("romfs:/sound/Back.wav", &wav_effect, &audioBuffer_effect)) {
        playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
    }
}

void Play_against_wall(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {

    if (!loadWav("romfs:/sound/Bond.wav", &wav_effect, &audioBuffer_effect)) {
        exitGraphics();
        exitAudio();
    }

    bool collision = false;
    float speed[2];   // Movement speed
    float x;
    float y;
    float x_racket = 162.0f;

    // Buffer for bottom screen text
    CreateBuffer(&g_htp_pauseBuf, &g_htp_pauseText, Htp_pause, font);

    ballInit(&x, &y, speed);
    
    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_Y) {
            break; // Break in order to return to the main loop
        }
        if (kHeld & KEY_LEFT) {
            if (x_racket > 0.0f) {
                    x_racket -= RACKET_SPEED;
                }
        }
        if (kHeld & KEY_RIGHT) {
            if (x_racket + RACKET_LENGTH < SCREEN_SIZE_LENGTH) {
                    x_racket += RACKET_SPEED;
                }
        }

        Movement(&x, &y, speed);

        if (kDown & KEY_SELECT) {
            Pause(top, bottom);
        }
        if (x <= 0.0f || x >= SCREEN_SIZE_LENGTH - BALL_SIDE) speed[0] = -speed[0], collision = true;
        if (y <= 0.0f) speed[1] = -speed[1], collision = true;

        // Collision racket
        if (y + BALL_SIDE >= SCREEN_SIZE_WIDTH - RACKET_WIDTH) {
            if ((x + (BALL_SIDE/2.0f) >= x_racket) & (x + (BALL_SIDE/2.0f) <= x_racket + RACKET_LENGTH)) {
                speed[1] = -speed[1];
                collision = true;
            } else {
                ballInit(&x, &y, speed);
            }
        }

        ImpSpeedCol(collision, speed);
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            // Top screen
            C2D_SceneBegin(top);
            C2D_DrawImageAt(images[0], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Map
            C2D_DrawImageAt(images[4], x_racket, SCREEN_SIZE_WIDTH-RACKET_WIDTH, DEPTH, NULL, 1.0f, 1.0f); // Racket
            C2D_DrawImageAt(images[3], x, y, DEPTH, NULL, 1.0f, 1.0f); // Ball

            // Bottom screen
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_htp_pauseText, C2D_AtBaseline | C2D_WithColor, BOTTOM_TEXT_X, BOTTOM_TEXT_Y, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }
        C3D_FrameEnd(0);

        gfxFlushBuffers();

        if (collision) {
            collision = false;
            playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
        }
    }
    C2D_TextBufDelete(g_htp_pauseBuf);
    CreateMainMenuBufferTop();
    CreateMainMenuBufferBottom();
    if (loadWav("romfs:/sound/Back.wav", &wav_effect, &audioBuffer_effect)) {
        playWav(&wav_effect, audioBuffer_effect, &waveBuf_effect,1, false);
    }
}