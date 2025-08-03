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
#define MAXPROJECTILES 30
#define PROJECTILE_UPDATES_PER_FRAME 10
// How fast character rotates
#define ROTSPEED PI / 80

#define ENEMY_UPDATES_PER_FRAME 60

#endif