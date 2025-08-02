#include "screen.h"
#include <raylib.h>
#include "movement.h"
#include "sprites.h"
#include <stdio.h>


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

void drawHud(void *player, void *weapon, int wpnn, int remaingingEnemies)
{
    Player player1 = *(Player *)player;

    Weapon wpn = *(Weapon *)weapon;

    float hudHeightScale = 0.8f * (float)SCREEN_HEIGHT / 1080.0; // An attempt to make the hud scale to different aspect ratios. For your own sanity, don't

    // Draw the hud background
    DrawRectangle(0, (SCREEN_HEIGHT - 90 * hudHeightScale) - 4, SCREEN_WIDTH, 90 * hudHeightScale, WHITE);
    DrawRectangle(0, SCREEN_HEIGHT - 90 * hudHeightScale, SCREEN_WIDTH, 90 * hudHeightScale, CERISE);

    // Draw konglig doomguy
    Rectangle src = {
        0, 0, Sprites[UI_GUY].width, Sprites[UI_GUY].height};
    Rectangle dest = {
        (SCREEN_WIDTH - Sprites[UI_GUY].width * hudHeightScale) / 2,
        SCREEN_HEIGHT - hudHeightScale * Sprites[UI_GUY].height,
        Sprites[UI_GUY].width * hudHeightScale,
        Sprites[UI_GUY].height * hudHeightScale};
    DrawTexturePro(Sprites[UI_GUY], src, dest, (Vector2){0.0, 0.0}, 0.0f, WHITE);

    // Make an offset to the right of konglig doomguy
    src = (Rectangle){0, 0, Sprites[UI_SELECT1].width, Sprites[UI_SELECT1].height};
    dest = (Rectangle){(SCREEN_WIDTH + Sprites[UI_GUY].width * hudHeightScale) / 2, SCREEN_HEIGHT - hudHeightScale * Sprites[UI_GUY].height, Sprites[UI_SELECT1].width, Sprites[UI_SELECT1].height};

    // Draw the corresponding weapon select sprite
    switch (wpnn)
    {
    case 0:
        DrawTexturePro(Sprites[UI_SELECT1], src, dest, (Vector2){0.0, 0.0}, 0.0f, WHITE);
        break;
    case 1:
        DrawTexturePro(Sprites[UI_SELECT2], src, dest, (Vector2){0.0, 0.0}, 0.0f, WHITE);
        break;
    case 2:
        DrawTexturePro(Sprites[UI_SELECT3], src, dest, (Vector2){0.0, 0.0}, 0.0f, WHITE);
        break;
    default:
        break;
    }
    // make three black squares
    DrawRectangle(((SCREEN_WIDTH + Sprites[UI_GUY].width * hudHeightScale) / 2) + Sprites[UI_SELECT1].width + 4, SCREEN_HEIGHT - 90 * hudHeightScale + 4, 300, 90 * hudHeightScale - 8, BLACK);
    DrawRectangle(((SCREEN_WIDTH - Sprites[UI_GUY].width * hudHeightScale) / 2) - 204, SCREEN_HEIGHT - 90 * hudHeightScale + 4, 200, 90 * hudHeightScale - 8, BLACK);
    DrawRectangle(4, SCREEN_HEIGHT - 90 * hudHeightScale + 4, 450, 90 * hudHeightScale - 8, BLACK);

    // Draw some text in the squares
    char buffer[64];
    sprintf(buffer, "HP: %d", player1.hp);
    DrawTextEx(jupiter, buffer, (Vector2){((SCREEN_WIDTH - Sprites[UI_GUY].width * hudHeightScale) / 2) - 200, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);

    sprintf(buffer, "+");
    DrawText(buffer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, (Color){245, 40, 145, 204});

    if (wpnn == 0)
        sprintf(buffer, "AMMO: inf");
    else
        sprintf(buffer, "AMMO: %d", wpn.ammo);

    DrawTextEx(jupiter, buffer, (Vector2){((SCREEN_WIDTH + Sprites[UI_GUY].width * hudHeightScale) / 2) + Sprites[UI_SELECT1].width + 8, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);

    sprintf(buffer, "REMAINING 0an: %d", remaingingEnemies);
    DrawTextEx(jupiter, buffer, (Vector2){8, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);
}