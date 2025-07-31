#ifndef SPRITES_H
#define SPRITES_H
#include <raylib.h>

int initSprites();

void destroySprites();

#define NUM_OF_SPRITES 17

typedef enum SpriteNames
{
    MAP_WALL,
    MAP_WALLVAR1,
    MAP_WALLVAR2,
    MAP_FLOOR,
    MAP_CEILING,

    ENEMY_MELEE,
    ENEMY_MIDRANGE,
    ENEMY_SNIPER,
    ENEMY_PROJECTILE,
    ENEMY_DEAD,

    ALLY_HEALTH,
    ALLY_AMMO,
    ALLY_PROJECTILE,

    UI_SELECT1,
    UI_SELECT2,
    UI_SELECT3,
    UI_GUY,

} SpriteNames;

extern const char *SpritePaths[];
extern Texture2D Sprites[];

#endif