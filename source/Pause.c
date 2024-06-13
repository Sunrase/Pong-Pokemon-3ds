#include "common.h"
#include "Pause.h"
#include "Buffer.h"

void Pause(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    CreateBuffer(&g_pauseBuf, &g_pauseText, pause, font);

    CreateBuffer(&g_resumeBuf, &g_resumeText, pause_resume, font);

    CreateBuffer(&g_YnotBuf, &g_YnotText, Y_not, font);

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
            C2D_DrawText(&g_pauseText, C2D_AtBaseline | C2D_WithColor, SCREEN_SIZE_LENGTH/4.0f-5.0f, SCREEN_SIZE_WIDTH/2.0f, DEPTH, 1.0f, 1.0f, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            C2D_DrawText(&g_resumeText, C2D_AtBaseline | C2D_WithColor, 2.0f, SCREEN_SIZE_WIDTH/2.0f+35.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            if (kDown & KEY_Y) {
                C2D_SceneBegin(top);
                C2D_DrawRectSolid(10.0f, 95.0f, DEPTH, RECTANGLE_LENGTH, RECTANGLE_WIDTH, C2D_Color32(0xD8, 0xC8, 0x9D, 0xFF));
                C2D_DrawText(&g_YnotText, C2D_AtBaseline | C2D_WithColor, 15.0f, 122.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, C2D_Color32(0x6B, 0x7A, 0x97, 0xFF));
            }     
        }
        C3D_FrameEnd(0);
        gfxFlushBuffers();
    }
    C2D_TextBufDelete(g_pauseBuf);
    C2D_TextBufDelete(g_resumeBuf);
    C2D_TextBufDelete(g_YnotBuf);
}