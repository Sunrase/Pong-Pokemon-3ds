#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <citro3d.h>
#include <citro2d.h>
#include <3ds/ndsp/ndsp.h>
#include <3ds/services/fs.h>

#define RACKET_SPEED 3.0f
#define ANGLE 60
#define FLAT_ANGLE 180
#define BALL_SIDE 25.0f
#define MAX_IMAGES 4
#define SCREEN_SIZE_WIDTH   240.0f
#define SCREEN_SIZE_LENGTH  400.0f
#define RACKET_WIDTH 25.0f
#define RACKET_LENGTH 75.0f
#define DEPTH 1.0f
#define SPEED_INCREMENT 0.1f
#define MIN_SPEED 0.75f
#define ADDED_SPEED 0.5f

C2D_Image images[MAX_IMAGES];
C2D_SpriteSheet spriteSheet;
C2D_TextBuf g_dynamicBuf, g_dynamicBuf_2;
C2D_Text g_dynamicText, g_dynamicText_2;

void initGraphics() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    romfsInit();
    consoleInit(GFX_BOTTOM, NULL);

    // Score text buffer
    g_dynamicBuf = C2D_TextBufNew(3);
    g_dynamicBuf_2 = C2D_TextBufNew(3);
}

void exitGraphics() {
    C2D_TextBufDelete(g_dynamicBuf);
    C2D_TextBufDelete(g_dynamicBuf_2);
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
    if (((speed[0] < MIN_SPEED) & (speed[1] > 0.0f)) || ((speed[0] > -MIN_SPEED) & (speed[1] < 0.0f))) {
        if ((speed[0] < MIN_SPEED) & (speed[0] > 0.0f)) speed[0] += ADDED_SPEED;
        else speed[0] -= ADDED_SPEED;
    }
    if (((speed[1] < MIN_SPEED) & (speed[1] > 0.0f)) || ((speed[1] > -MIN_SPEED) & (speed[1] < 0.0f))) {
        if ((speed[1] < MIN_SPEED) & (speed[1] > 0.0f)) speed[0] += ADDED_SPEED;
        else speed[1] -= ADDED_SPEED;
    }
    *x = SCREEN_SIZE_LENGTH/2.0f - BALL_SIDE/2.0f; // Ball positions
    *y = SCREEN_SIZE_WIDTH/2.0f - BALL_SIDE/2.0f; // Ball positions
}

void ImpSpeedCol(bool collision, float speed[2]) {
    // Increase speed when collision
    if (collision & (speed[0] > 0.0f)) speed[0] += SPEED_INCREMENT;
    if (collision & (speed[0] < 0.0f)) speed[0] -= SPEED_INCREMENT;
    if (collision & (speed[1] > 0.0f)) speed[1] += SPEED_INCREMENT;
    if (collision & (speed[1] < 0.0f)) speed[1] -= SPEED_INCREMENT;
}

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

void Movement(float* x, float* y, float speed[2]) {
    *x += speed[0];
    *y += speed[1];
}

int main(int argc, char **argv) {
    bool collision = false;
    float speed[2];   // Movement speed
    float x;
    float y;
    float y_racket_1 = SCREEN_SIZE_WIDTH/2.0f - RACKET_LENGTH/2.0f;
    float y_racket_2 = SCREEN_SIZE_WIDTH/2.0f - RACKET_LENGTH/2.0f;
    int score_player_1 = 0;
    int score_player_2 = 0;

    ballInit(&x, &y, speed);

    // Target render for top screen
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    // Initialisation
    initGraphics();

    // Charger l'image
    loadImages();

    // Boucle principale
    while (aptMainLoop()) {

        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break; // Break in order to return to Hb Menu
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

        Movement(&x, &y, speed);

        // Detect collisions and reverse y axe
        if (y <= 0.0f || y >= SCREEN_SIZE_WIDTH - BALL_SIDE) speed[1] = -speed[1], collision = true;

        // Collision left racket
        if (x <= RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_1) & (y - (BALL_SIDE/2.0f) <= y_racket_1 + RACKET_LENGTH)) {
                speed[0] = -speed[0];
                collision = true;
            } else {
                score_player_1 += 1;
                ballInit(&x, &y, speed);
            }
        }

        // Collision right racket
        if (x + RACKET_WIDTH >= SCREEN_SIZE_LENGTH - RACKET_WIDTH) {
            if ((y + (BALL_SIDE/2.0f) >= y_racket_2) & (y - (BALL_SIDE/2.0f) <= y_racket_2 + RACKET_LENGTH)) {
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

        // Debogage
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
            C2D_DrawImageAt(images[3], 0.0f, 0.0f, DEPTH, NULL, 1.0f, 1.0f); // Map
            C2D_DrawImageAt(images[0], 0.0f, y_racket_1, DEPTH, NULL, 1.0f, 1.0f); // Left racket
            C2D_DrawImageAt(images[1], SCREEN_SIZE_LENGTH - RACKET_WIDTH, y_racket_2, DEPTH, NULL, 1.0f, 1.0f); // Right racket
            C2D_DrawText(&g_dynamicText, C2D_AtBaseline | C2D_WithColor, 220.0f, 40.0f, 1.0f, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF)); // Scores
            C2D_DrawText(&g_dynamicText_2, C2D_AtBaseline | C2D_WithColor, 166.0f, 40.0f, 1.0f, 1.0f, 1.0f, C2D_Color32(0xE8, 0xE9, 0xE3, 0xFF));
            C2D_DrawImageAt(images[2], x, y, DEPTH, NULL, 1.0f, 1.0f); // Ball
        }

        // End of render
        C3D_FrameEnd(0);

        gfxFlushBuffers();

        if (collision) collision = false;
    }

    exitGraphics();
    return 0;
}