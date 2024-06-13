#include "common.h"
#include "Graphics.h"

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
    C2D_FontFree(font);
    romfsExit();
    C2D_Fini();
    C3D_Fini();
    gfxExit();
}

void loadImages(C2D_SpriteSheet spriteSheet, C2D_Image images[], int max_image, C2D_Font font) {
    int imageCount = 0;
    // Load the sprite sheet
    spriteSheet = C2D_SpriteSheetLoad("romfs:/images.t3x");
    if (!spriteSheet) {
        printf("Failed to load sprite sheet\n");
        exitGraphics(font);
        return;
    }

    // Load each image individually from the sprite sheet
    for (int i = 0; i < max_image; i++) {
        C2D_Image image = C2D_SpriteSheetGetImage(spriteSheet, i);
        if (image.tex) {
            images[imageCount++] = image;
        } else {
            // If image is not found, stop loop
            break;
        }
    }
}

void LoadFont(C2D_Font* font) {
    *font = C2D_FontLoad("romfs:/PressStart2P-vaV7.bcfnt");
    if (!*font) {
        printf("Erreur lors du chargement de la police : PressStart2P.ttf\n");
        return;
    }
}

