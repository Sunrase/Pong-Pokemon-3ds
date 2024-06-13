#ifndef GRAPHICS_H
#define GRAPHICS_H

void initGraphics();
void exitGraphics();
void loadImages(C2D_SpriteSheet spriteSheet, C2D_Image images[], int max_image, C2D_Font font);
void LoadFont(C2D_Font* font);

#endif