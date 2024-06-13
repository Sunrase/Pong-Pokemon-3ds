#include "common.h"
#include "Buffer.h"

void CreateBuffer(C2D_TextBuf *g_Buffer, C2D_Text *g_Text, const char* Text, C2D_Font font) {
    *g_Buffer = C2D_TextBufNew(strlen(Text) + 1);
    C2D_TextFontParse(g_Text, font, *g_Buffer, Text);
    C2D_TextOptimize(g_Text);
}

void DeleteMainMenuBuffer() {
    C2D_TextBufDelete(g_botMMBuf);
    C2D_TextBufDelete(g_staticBuf);
    C2D_TextBufDelete(g_playermodeBuf);
    C2D_TextBufDelete(g_playerwallBuf);
    C2D_TextBufDelete(g_unbeatableAIBuf);
}

void CreateMainMenuBufferTop() {
    // Choose a game mode
    CreateBuffer(&g_staticBuf, &g_staticText, CGM, font);

    // Play with another player 
    CreateBuffer(&g_playermodeBuf, &g_playermodeText, playermode, font);

    // Play against the wall
    CreateBuffer(&g_playerwallBuf, &g_playerwallText, playerwall, font);

    // Play against the unbeatable AI
    CreateBuffer(&g_unbeatableAIBuf, &g_unbeatableAIText, unbeatableAI, font);
}

void CreateMainMenuBufferBottom() {
    CreateBuffer(&g_botMMBuf, &g_botMMText, Bot_MM, font);
}