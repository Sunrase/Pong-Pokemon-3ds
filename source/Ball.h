#ifndef BALL_H
#define BALL_H

void ballInit(float* x, float* y, float speed[2]);

void ImpSpeedCol(bool collision, float speed[2]);

void Movement(float* x, float* y, float speed[2]);

#endif