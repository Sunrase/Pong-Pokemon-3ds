#include "common.h"
#include "animations.h"

void PointerAnimation(float *x, float *mov) {
    if (*x <= X_POINTER_POS) *mov = -*mov;
    else if (*x >= X_POINTER_POS+5.0f) *mov = -*mov;
    *x += *mov;
}

void LogoAnimation(float *y_logo, float *mov_logo) {
    if (*y_logo <= Y_LOGO) *mov_logo = -*mov_logo;
    else if (*y_logo >= Y_LOGO+10.0f) *mov_logo = -*mov_logo;
    *y_logo += *mov_logo;
}