#ifndef SCREEN_H
#define SCREEN_H
#include <raylib.h>
#include "config.h"

#ifndef CONFIG_H
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NUM_RAYS SCREEN_WIDTH
#define FOV 90.0f
#define FPS_TARGET 120
#define CROSSHAIRSCALE 1.5
#endif

typedef enum
{
    MAINMENU,
    GAMEPLAY,
    PAUSEMENU,
    ENDSCREEN,
    DEATHSCREEN,
    THEEND
} GameState;

#define CERISE (Color){230, 65, 133, 255}

extern Font jupiter;
extern Font textFont;

void drawWeapon(void *wpns, int wpnid);

void drawHud(void *player, void *weapon, int wpnn, int remaingingEnemies, int frames);

void drawScene(void *player, void **enemyData, int enemycount, void **wallData, int raycount, void **projData, Image *floorImage, Texture2D *floorTextureBuffer, Image floorTexture, Image roofTexture);

void drawMenu(GameState gameState);

#endif