#include "sprites.h"

Texture2D Sprites[NUM_OF_SPRITES] = {0};

int initSprites()
{
    for (int i = 0; i < NUM_OF_SPRITES; i++)
    {
        Sprites[i] = LoadTexture(SpritePaths[i]);
    }
    return 1;
}

void destroySprites()
{
    for (int i = 0; i < NUM_OF_SPRITES; i++)
    {
        UnloadTexture(Sprites[i]);
    }
    return;
}

const char *SpritePaths[] = {
    "Data/Sprites/Wall.png",
    "Data/Sprites/WallVar.png",
    "Data/Sprites/WallVar2.png",
    "Data/Sprites/Ground.png",
    "Data/Sprites/Sky.png",

    "Data/Sprites/MeleeNollantransp.png",
    "Data/Sprites/MidrangeNollantransp.png",
    "Data/Sprites/LongRangeNollan.png",
    "Data/Sprites/Projectiles/EvilProjectile.png",
    "Data/Sprites/Nollekorttransp.png",

    "Data/Sprites/Health.png",
    "Data/Sprites/Ammo.png",
    "Data/Sprites/Projectiles/largerprojectiletransp.png",

    "Data/Sprites/HUD/Weaponselect1.png",
    "Data/Sprites/HUD/Weaponselect2.png",
    "Data/Sprites/HUD/Weaponselect3.png",
    "Data/Sprites/HUD/85ed57ab85bbe08a0edfd3cfa5edfc38.jpg",

};