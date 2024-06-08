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
#define MAX_IMAGES 5
#define SCREEN_SIZE_WIDTH   240.0f
#define SCREEN_SIZE_LENGTH  400.0f
#define RACKET_WIDTH 25.0f
#define RACKET_LENGTH 75.0f
#define DEPTH 1.0f
#define SPEED_INCREMENT 0.1f
#define MIN_SPEED 0.75f
#define ADDED_SPEED 0.5f
#define MIDDLE_OF_RACKET 34.0f // In reality 75/2 = 37.5 but it tends to corrupt the racket sprite
#define RECTANGLE_WIDTH 35.0f
#define RECTANGLE_LENGTH 360.0f
#define BOTTOM_TEXT_Y 240.0f/2.0f - 50.0f
#define BOTTOM_TEXT_X 400.0f/15.0f

C2D_Image images[MAX_IMAGES];
C2D_SpriteSheet spriteSheet;
C2D_TextBuf g_dynamicBuf, g_dynamicBuf_2, g_staticBuf, g_pauseBuf, g_htp_pauseBuf, g_resumeBuf, g_YnotBuf, g_botMMBuf;
C2D_Text g_dynamicText, g_dynamicText_2, g_staticText, g_pauseText, g_htp_pauseText, g_resumeText, g_YnotText, g_botMMText;
static const char MainText[] = "Choose a game mode :\n\nPress A to play \nwith another player\n\nPress B to play\nwith the wall\n";
static const char pause[] = "Pause\n";
static const char pause_resume[] = "Press Select to resume";
static const char Htp_pause[] = "Press Select to pause \nthe game\n\nPress Y to return \nto the main menu\n";
static const char Y_not[] = "Resume the game to do that\n";
static const char Bot_MM[] = "Press Start to exit";

void initGraphics() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    romfsInit();
    // Enable for debugging
    //consoleInit(GFX_BOTTOM, NULL);
}

void exitGraphics() {
    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

// GRAPHICS
void loadImages() {
    int imageCount = 0;
    // Load the sprite sheet
    spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/images.t3x");
    if (!spriteSheet) {
        printf("Failed to load sprite sheet\n");
        exitGraphics();
        return;
    }

    // Load each image individually from the sprite sheet
    for (int i = 0; i < MAX_IMAGES; i++) {
        C2D_Image image = C2D_SpriteSheetGetImage(spriteSheet, i);
        if (image.tex) {
            images[imageCount++] = image;
        } else {
            // If image is not found, stop loop
            break;
        }
    }
}

void Pause(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    g_pauseBuf = C2D_TextBufNew(sizeof(pause));
    g_resumeBuf = C2D_TextBufNew(sizeof(pause_resume));
    g_YnotBuf = C2D_TextBufNew(sizeof(Y_not));

    C2D_TextParse(&g_pauseText, g_pauseBuf, pause);
    C2D_TextOptimize(&g_pauseText);

    C2D_TextParse(&g_resumeText, g_resumeBuf, pause_resume);
    C2D_TextOptimize(&g_resumeText);

    C2D_TextParse(&g_YnotText, g_YnotBuf, Y_not);
    C2D_TextOptimize(&g_YnotText);
    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_SELECT) {
            break;
        }
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_pauseText, C2D_AtBaseline | C2D_WithColor, SCREEN_SIZE_LENGTH/4.0f, SCREEN_SIZE_WIDTH/2.0f, DEPTH, 1.5f, 1.5f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawText(&g_resumeText, C2D_AtBaseline | C2D_WithColor, SCREEN_SIZE_LENGTH/20.0f, SCREEN_SIZE_WIDTH/2.0f+35.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            if (kDown & KEY_Y) {
                C2D_SceneBegin(top);
                C2D_DrawRectSolid(20.0f, 95.0f, DEPTH, RECTANGLE_LENGTH, RECTANGLE_WIDTH, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
                C2D_DrawText(&g_YnotText, C2D_AtBaseline | C2D_WithColor, 25.0f, 122.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            }     
        }
        C3D_FrameEnd(0);
        gfxFlushBuffers();
    }
    C2D_TextBufDelete(g_pauseBuf);
    C2D_TextBufDelete(g_resumeBuf);
    C2D_TextBufDelete(g_YnotBuf);
}

void ballInit(float* x, float* y, float speed[2]) {
    // Direction
    srand( time( NULL ) );
    int choix = rand() % 1;
    if (choix == 0) {
        speed[0] = cos(rand()%(ANGLE + ANGLE + 1) -ANGLE);
        speed[1] = sin(rand()%(ANGLE + ANGLE + 1) -ANGLE);
    } else {
        speed[0] = cos(rand()%(ANGLE + ANGLE + 1) -ANGLE);
        speed[1] = sin(rand()%(FLAT_ANGLE+ANGLE + (FLAT_ANGLE-ANGLE) + 1) -(FLAT_ANGLE-ANGLE));
    }
    // If too slow
    if (((speed[0] < MIN_SPEED) & (speed[0] > 0.0f)) || ((speed[0] > -MIN_SPEED) & (speed[0] < 0.0f))) {
        if ((speed[0] < MIN_SPEED) & (speed[0] > 0.0f)) speed[0] += ADDED_SPEED;
        else speed[0] -= ADDED_SPEED;
    }
    if (((speed[1] < MIN_SPEED) & (speed[1] > 0.0f)) || ((speed[1] > -MIN_SPEED) & (speed[1] < 0.0f))) {
        if ((speed[1] < MIN_SPEED) & (speed[1] > 0.0f)) speed[1] += ADDED_SPEED;
        else speed[1] -= ADDED_SPEED;
    }
    *x = SCREEN_SIZE_LENGTH/2.0f - 13.0f; // Ball positions
    *y = SCREEN_SIZE_WIDTH/2.0f - 13.0f; // Ball positions
}

// Increase speed when collision
void ImpSpeedCol(bool collision, float speed[2]) {
    if (collision & (speed[0] > 0.0f)) speed[0] += SPEED_INCREMENT;
    if (collision & (speed[0] < 0.0f)) speed[0] -= SPEED_INCREMENT;
    if (collision & (speed[1] > 0.0f)) speed[1] += SPEED_INCREMENT;
    if (collision & (speed[1] < 0.0f)) speed[1] -= SPEED_INCREMENT;
}

// Update score and render it
void UpdateScoreText(int number, int number_2) {
    char numberStr[3];
    char numberStr_2[3];
    // Score 1
    snprintf(numberStr, sizeof(numberStr), "%d", number);

    C2D_TextBufClear(g_dynamicBuf);
    C2D_TextParse(&g_dynamicText, g_dynamicBuf, numberStr);
    C2D_TextOptimize(&g_dynamicText);

    // Score 2
    snprintf(numberStr_2, sizeof(numberStr_2), "%d", number_2);

    C2D_TextBufClear(g_dynamicBuf_2);
    C2D_TextParse(&g_dynamicText_2, g_dynamicBuf_2, numberStr_2);
    C2D_TextOptimize(&g_dynamicText_2);
}

// Move the ball
void Movement(float* x, float* y, float speed[2]) {
    *x += speed[0];
    *y += speed[1];
}

//Game Modes
void Play_with_another_player(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    
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
    g_htp_pauseBuf = C2D_TextBufNew(sizeof(Htp_pause));

    // Bottom screen text
    C2D_TextParse(&g_htp_pauseText, g_htp_pauseBuf, Htp_pause);
    C2D_TextOptimize(&g_htp_pauseText);

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
            C2D_DrawText(&g_htp_pauseText, C2D_AtBaseline | C2D_WithColor, BOTTOM_TEXT_X, BOTTOM_TEXT_Y, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }

        // End of render
        C3D_FrameEnd(0);

        gfxFlushBuffers();

        if (collision) collision = false;
    }
    C2D_TextBufDelete(g_dynamicBuf);
    C2D_TextBufDelete(g_dynamicBuf_2);
    C2D_TextBufDelete(g_htp_pauseBuf);
}





void Play_against_wall(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    bool collision = false;
    float speed[2];   // Movement speed
    float x;
    float y;
    float x_racket = 162.0f;

    // Buffer for bottom screen text
    g_htp_pauseBuf = C2D_TextBufNew(sizeof(Htp_pause));

    // Bottom screen text
    C2D_TextParse(&g_htp_pauseText, g_htp_pauseBuf, Htp_pause);
    C2D_TextOptimize(&g_htp_pauseText);

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
            C2D_DrawText(&g_htp_pauseText, C2D_AtBaseline | C2D_WithColor, BOTTOM_TEXT_X, BOTTOM_TEXT_Y, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }
        C3D_FrameEnd(0);
        gfxFlushBuffers();
        if (collision) collision = false;
    }
    C2D_TextBufDelete(g_htp_pauseBuf);
}

int main(int argc, char **argv) {

    // Target render for top and bottom screen
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Initialisation
    initGraphics();

    // Load Images
    loadImages();

    g_staticBuf = C2D_TextBufNew(sizeof(MainText));
    C2D_TextParse(&g_staticText, g_staticBuf, MainText);
    C2D_TextOptimize(&g_staticText);

    g_botMMBuf = C2D_TextBufNew(sizeof(Bot_MM));
    C2D_TextParse(&g_botMMText, g_botMMBuf, Bot_MM);
    C2D_TextOptimize(&g_botMMText);

    //Main Loop
    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // Break in order to return to Hb Menu
        if (kDown & KEY_A) {
            Play_with_another_player(top, bottom);
        }
        if (kDown & KEY_B) {
            Play_against_wall(top, bottom);
        }
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_staticText, C2D_AtBaseline | C2D_WithColor, SCREEN_SIZE_LENGTH/9.0f, SCREEN_SIZE_WIDTH/6.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
            C2D_DrawText(&g_botMMText, C2D_AtBaseline | C2D_WithColor, SCREEN_SIZE_LENGTH/8.0f, SCREEN_SIZE_WIDTH/2.0f+7.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
        }
        C3D_FrameEnd(0);
        gfxFlushBuffers();
    }
    C2D_TextBufDelete(g_botMMBuf);
    C2D_TextBufDelete(g_staticBuf);
    exitGraphics();

    return 0;
}