#include "common.h"
#include "Ball.h"

void ballInit(float* x, float* y, float speed[2]) {
    // Direction
    srand(time(NULL));
    int choix = rand() % 1;
    if (choix == 0) {
        speed[0] = cos(rand()%(ANGLE + ANGLE + 1) -ANGLE);
        speed[1] = sin(rand()%(ANGLE + ANGLE + 1) -ANGLE);
    } else {
        speed[0] = cos(rand()%(ANGLE + ANGLE + 1) -ANGLE);
        speed[1] = sin(rand()%(FLAT_ANGLE+ANGLE + (FLAT_ANGLE-ANGLE) + 1) -(FLAT_ANGLE-ANGLE));
    }
    // If too slow
    if ((speed[0] < MIN_SPEED) & (speed[0] > 0.0f)) speed[0] += ADDED_SPEED;
    if ((speed[0] > -MIN_SPEED) & (speed[0] < 0.0f)) speed[0] -= ADDED_SPEED;
    if ((speed[1] < MIN_SPEED) & (speed[1] > 0.0f)) speed[1] += ADDED_SPEED;
    if ((speed[1] > -MIN_SPEED) & (speed[1] < 0.0f)) speed[1] -= ADDED_SPEED;

    *x = SCREEN_SIZE_LENGTH/2.0f - 13.0f; // Ball positions
    *y = SCREEN_SIZE_WIDTH/2.0f - 13.0f; // Ball positions
}

// Increase speed when collision
void ImpSpeedCol(bool collision, float speed[2]) {
    if (collision & (speed[0] > 0.0f)) speed[0] += SPEED_INCREMENT;
    if (collision & (speed[0] < 0.0f)) speed[0] -= SPEED_INCREMENT;
    if (collision & (speed[1] > 0.0f)) speed[1] += SPEED_INCREMENT;
    if (collision & (speed[1] < 0.0f)) speed[1] -= SPEED_INCREMENT;
}

// Move the ball
void Movement(float* x, float* y, float speed[2]) {
    *x += speed[0];
    *y += speed[1];
}