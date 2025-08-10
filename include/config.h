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
// How fast character rotates
#define ROTSPEED PI / 80
// Makes aiming a bit more forgiving
#define PITYHITRADIUS 3.0
// How large the crosshair is
#define CROSSHAIRSCALE 5.0

// How many projectiles can exist before the oldest one gets deleted
#define MAXPROJECTILES 50
// How often do projectiles get updated
#define PROJECTILE_UPDATES_PER_FRAME 10

// How many enemyupdates happen per frame.
#define ENEMY_UPDATES_PER_FRAME 60
// Changes the weaponSprites
#define TAMEMODE

#define KEYBIND_SHOOT KEY_SPACE
#define KEYBIND_WEAPON1 '1'
#define KEYBIND_WEAPON2 '2'
#define KEYBIND_WEAPON3 '3'
#define KEYBIND_NEXTWEAPON 'E'
#define KEYBIND_PREVIOUSWEAPON 'Q'

#define KEYBIND_TURNLEFT KEY_LEFT
#define KEYBIND_TURNRIGHT KEY_RIGHT
#define KEYBIND_MOVEFORWARD 'W'
#define KEYBIND_MOVELEFT 'A'
#define KEYBIND_MOVEBACKWARD 'S'
#define KEYBIND_MOVERIGHT 'D'

#define KEYBIND_PAUSE KEY_ESCAPE
#define KEYBIND_PLAY KEY_ENTER
#define KEYBIND_EXIT KEY_BACKSPACE

#endif