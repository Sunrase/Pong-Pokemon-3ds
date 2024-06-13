#include "common.h"

C2D_Image images[MAX_IMAGES];
C2D_SpriteSheet spriteSheet;
C2D_Font font;
C2D_TextBuf g_dynamicBuf, g_dynamicBuf_2, g_staticBuf, g_pauseBuf, g_htp_pauseBuf, g_resumeBuf, g_YnotBuf, g_botMMBuf, g_playermodeBuf, g_playerwallBuf, g_unbeatableAIBuf;
C2D_Text g_dynamicText, g_dynamicText_2, g_staticText, g_pauseText, g_htp_pauseText, g_resumeText, g_YnotText, g_botMMText, g_playermodeText, g_playerwallText, g_unbeatableAIText;
WAV wav_background, wav_effect;
ndspWaveBuf waveBuf_background, waveBuf_effect;

void* audioBuffer_background = NULL;
void* audioBuffer_effect = NULL;

const char pause[] = "Pause\n";
const char pause_resume[] = "Press Select to resume";
const char Y_not[] = "Resume the game to do that\n";
const char CGM[] = "Choose a game mode :";
const char playermode[] = "Play with another\nplayer";
const char playerwall[] = "Play with the wall";
const char unbeatableAI[] = "Play against the\nunbeatable AI";
const char Htp_pause[] = "Press Select to\npause the game\n\n\nPress Y to return \nto the main menu\n";
const char Bot_MM[] = "Press Start to exit";