#include "common.h"
#include "relative_to_game.h"

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