#ifndef SPRITES_H
#define SPRITES_H
#include <raylib.h>

int initSprites();

void destroySprites();

#define NUM_OF_SPRITES 20

typedef enum SpriteNames
{
    MAP_WALL,
    MAP_WALLVAR1,
    MAP_WALLVAR2,
    MAP_FLOOR,
    MAP_CEILING,

    ENEMY_MELEE,
    ENEMY_MELLE_DEAD,
    ENEMY_MIDRANGE,
    ENEMY_MIDRANGE_DEAD,
    ENEMY_SNIPER,
    ENEMY_SNIPER_DEAD,
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