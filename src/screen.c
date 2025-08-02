#include "screen.h"
#include <raylib.h>
#include "movement.h"

void drawWeapon(void *wpns, int wpnid)
{

    Weapon *w = (Weapon *)wpns;

    switch (w[wpnid].currentCooldown > (w[wpnid].baseCooldown / 2)) // draws Different sprite depending on cooldown
    {
    case 0:
    {
        Rectangle src = {
            0, 0,
            (float)w[wpnid].normalSprite.width,
            (float)w[wpnid].normalSprite.height};

        Rectangle dest = {
            w[wpnid].screenPos.x,
            SCREEN_HEIGHT - (w[wpnid].normalSprite.height * w[wpnid].normalScale.y) + w[wpnid].screenPos.y,
            w[wpnid].normalSprite.width * w[wpnid].normalScale.x,
            w[wpnid].normalSprite.height * w[wpnid].normalScale.y};

        DrawTexturePro(w[wpnid].normalSprite, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        break;
    }
    default:
    {
        Rectangle src = {
            0, 0,
            (float)w[wpnid].shootingSprite.width,
            (float)w[wpnid].shootingSprite.height};

        Rectangle dest = {
            w[wpnid].screenPos.x,
            SCREEN_HEIGHT - (w[wpnid].shootingSprite.height * w[wpnid].shootingScale.y) + w[wpnid].screenPos.y,
            w[wpnid].shootingSprite.width * w[wpnid].shootingScale.x,
            w[wpnid].shootingSprite.height * w[wpnid].shootingScale.y};

        DrawTexturePro(w[wpnid].shootingSprite, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
        break;
    }
    }
}
