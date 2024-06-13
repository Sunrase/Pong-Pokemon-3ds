#ifndef CREATE_BUFFER_H
#define CREATE_BUFFER_H

#include <citro2d.h>

void CreateBuffer(C2D_TextBuf *g_Buffer, C2D_Text *g_Text, const char* Text, C2D_Font font);

void DeleteMainMenuBuffer();

void CreateMainMenuBufferTop();

void CreateMainMenuBufferBottom();

#endif