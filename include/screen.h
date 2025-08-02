#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NUM_RAYS SCREEN_WIDTH
#define FOV 90.0f
#define FPS_TARGET 120

extern Font jupiter;
extern Font textFont;

void drawWeapon(void *wpns, int wpnid);

#endif