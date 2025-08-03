#include "sprites.h"
#include "screen.h"

Font jupiter;
Font textFont;

Texture2D Sprites[NUM_OF_SPRITES] = {0};

int initSprites()
{
    for (int i = 0; i < NUM_OF_SPRITES; i++)
    {
        Sprites[i] = LoadTexture(SpritePaths[i]);
    }
    jupiter = LoadFont("Data/Sprites/HUD/fonts/jupiter_crash.png");
    textFont = LoadFont("Data/Sprites/Fonts/setback.png");
    return 1;
}

void destroySprites()
{
    for (int i = 0; i < NUM_OF_SPRITES; i++)
    {
        UnloadTexture(Sprites[i]);
    }
    UnloadFont(jupiter);
    UnloadFont(textFont);
    return;
}

const char *SpritePaths[] = {
    "Data/Sprites/Map/Wall.png",
    "Data/Sprites/Map/WallVar.png",
    "Data/Sprites/Map/WallVar2.png",
    "Data/Sprites/Map/Ground.png",
    "Data/Sprites/Map/Sky.png",

    "Data/Sprites/Enemies/MeleeNollantransp.png",
    "Data/Sprites/Enemies/MeleeDead.png",
    "Data/Sprites/Enemies/MidrangeNollantransp.png",
    "Data/Sprites/Enemies/MidrangeDead.png",
    "Data/Sprites/Enemies/LongRangeNollan.png",
    "Data/Sprites/Enemies/LongRangeDead.png",
    "Data/Sprites/Projectiles/EvilProjectile.png",
    "Data/Sprites/Enemies/Nollekorttransp.png",

    "Data/Sprites/Health.png",
    "Data/Sprites/Ammo.png",
    "Data/Sprites/Projectiles/largerprojectiletransp.png",

    "Data/Sprites/HUD/Weaponselect1.png",
    "Data/Sprites/HUD/Weaponselect2.png",
    "Data/Sprites/HUD/Weaponselect3.png",
    "Data/Sprites/HUD/85ed57ab85bbe08a0edfd3cfa5edfc38.jpg",

    "Data/Sprites/Weapons/Fist1transp.png",
    "Data/Sprites/Weapons/Fist2transp.png",
    "Data/Sprites/Weapons/kpisttransp.png",
    "Data/Sprites/Weapons/Tame Weapons/kpist2transpVar.png",
    "Data/Sprites/Weapons/Projectile1transp.png",
    "Data/Sprites/Weapons/Fist2transp.png",

};