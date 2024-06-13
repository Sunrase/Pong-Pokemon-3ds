#ifndef MENUS_H
#define MENUS_H

#include <citro2d.h>

void MainMenu(C3D_RenderTarget* top, C3D_RenderTarget* bottom);

void StartGame(int selection, C3D_RenderTarget* top, C3D_RenderTarget* bottom);

void TitleScreen(C3D_RenderTarget* top, C3D_RenderTarget* bottom, float *y_logo, float *mov_logo, int *frame_count);

#endif