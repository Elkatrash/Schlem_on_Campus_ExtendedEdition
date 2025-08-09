#include <raylib.h>

#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NUM_RAYS SCREEN_WIDTH
#define FOV 90.0f
#define FPS_TARGET 120

// player maxhp
#define MAXHP 100
// player speed
#define MAXSPEED 800
// How many projectiles can exist before the oldest one gets deleted
#define MAXPROJECTILES 50
// How often do projectiles get updated
#define PROJECTILE_UPDATES_PER_FRAME 10
// How fast character rotates
#define ROTSPEED PI / 80
// Makes aiming a bit more forgiving
#define PITYHITRADIUS 3.0
// How many enemyupdates happen per frame.
#define ENEMY_UPDATES_PER_FRAME 60
// Changes the weaponSprites
#define TAMEMODE

#endif