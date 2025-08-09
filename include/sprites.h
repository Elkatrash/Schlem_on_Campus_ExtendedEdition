#ifndef SPRITES_H
#define SPRITES_H
#include <raylib.h>
#include "config.h"

int initSprites();

void destroySprites();

#define NUM_OF_SPRITES 26

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

    WEAPON_FIST_IDLE,
    WEAPON_FIST_PUNCH,
    WEAPON_KPIST_IDLE,
    WEAPON_KPIST_SHOOT,
    WEAPON_PIE_IDLE,
    WEAPON_PIE_SHOOT,

} SpriteNames;

extern const char *SpritePaths[];
extern Texture2D Sprites[];

#ifdef TAMEMODE
#define FIST1 "Data/Sprites/Weapons/Tame Weapons/Fist1transp.png"
#define KPIST2 "Data/Sprites/Weapons/Tame Weapons/kpist2transpVar.png"
#else
#define FIST1 "Data/Sprites/Weapons/Fist1transp.png"
#define KPIST2 "Data/Sprites/Weapons/kpist2transp.png"
#endif

#endif