#include "screen.h"
#include <raylib.h>
#include "movement.h"
#include "sprites.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

void drawHud(void *player, void *weapon, int wpnn, int remaingingEnemies, int frames)
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
    DrawRectangle(SCREEN_WIDTH - MeasureText("1234.5", 75) - 4, SCREEN_HEIGHT - 90 * hudHeightScale + 4, MeasureText("1234.5", 75), 90 * hudHeightScale - 8, BLACK);

    // Draw some text in the squares
    char buffer[64];
    sprintf(buffer, "HP: %d", player1.hp);
    DrawTextEx(jupiter, buffer, (Vector2){((SCREEN_WIDTH - Sprites[UI_GUY].width * hudHeightScale) / 2) - 200, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);

    const char *crosshair = "+";
    int crosshairSize = (int)(20 * CROSSHAIRSCALE);

    DrawText(crosshair, (SCREEN_WIDTH / 2) - (MeasureText(crosshair, crosshairSize) / 2), (SCREEN_HEIGHT / 2) - (crosshairSize / 2), crosshairSize, (Color){245, 40, 145, 204});

    (wpnn == 0) ? sprintf(buffer, "AMMO: inf") : sprintf(buffer, "AMMO: %d", wpn.ammo);

    DrawTextEx(jupiter, buffer, (Vector2){((SCREEN_WIDTH + Sprites[UI_GUY].width * hudHeightScale) / 2) + Sprites[UI_SELECT1].width + 8, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);

    sprintf(buffer, "REMAINING Oan: %d", remaingingEnemies);
    DrawTextEx(jupiter, buffer, (Vector2){8, SCREEN_HEIGHT - 90 * hudHeightScale + 4}, 75, 2, RED);

    sprintf(buffer, "%.1f", (float)frames / (float)FPS_TARGET);
    DrawTextEx(jupiter, buffer, (Vector2){SCREEN_WIDTH - MeasureText("1234.5", 75), SCREEN_HEIGHT - 90 * hudHeightScale + 8}, 75, 2, RED);
}

int compareEnemyDistance(const void *a, const void *b)
{
    // Load in thwo collisiondata pointers
    CollisionData *f1 = *(CollisionData **)a;
    CollisionData *f2 = *(CollisionData **)b;
    // Compare their distance
    if (!f1 || !f2)
        return -1;
    float cmp = f1->d - f2->d;
    if (cmp == 0.0)
        return 0;
    if (cmp < 0.0)
        return 1;
    return -1;
}

void drawScene(void *player, void **enemyData, int enemycount, void **wallData, int raycount, void **projData, Image *floorImage, Texture2D *floorTextureBuffer, Image floorTexture, Image roofTexture)
{
    Player p1 = *(Player *)player;
    CollisionData **enemyColl = (CollisionData **)enemyData;
    CollisionData **wallhits = (CollisionData **)wallData;
    CollisionData **projectileData = (CollisionData **)projData;

    // Group all the collisiondata into one huge array
    CollisionData **allData = malloc(sizeof(CollisionData *) * (enemycount + raycount + MAXPROJECTILES));
    if (!allData)
        return;
    memcpy(allData, enemyColl, enemycount * sizeof(CollisionData *));
    memcpy(allData + enemycount, wallhits, raycount * sizeof(CollisionData *));
    memcpy(allData + enemycount + raycount, projectileData, MAXPROJECTILES * sizeof(CollisionData *));

    // quicksort the array based on the distance of the collisions
    qsort(allData, (enemycount + raycount + MAXPROJECTILES), sizeof(CollisionData *), compareEnemyDistance);

    Color *pixels = floorImage->data; // Pointer to the Image pixel buffer

    Vec2 plane = {
        -p1.dir.y * tanf(DEG_TO_RAD(FOV / 2)),
        p1.dir.x * tanf(DEG_TO_RAD(FOV / 2))};

    float posScale = 0.0150f; // Achieved through trial and error. If the floor is moving to much in the same direction as the player. I.e moves infront of the player. This is to low and vice versa

    for (int y = SCREEN_HEIGHT / 2 + 1; y < SCREEN_HEIGHT; y++)
    {
        float rayDirX0 = p1.dir.x - plane.x;
        float rayDirY0 = p1.dir.y - plane.y;
        float rayDirX1 = p1.dir.x + plane.x;
        float rayDirY1 = p1.dir.y + plane.y;

        float rowDistance = (float)SCREEN_HEIGHT / (2.0f * y - SCREEN_HEIGHT);
        float stepX = rowDistance * (rayDirX1 - rayDirX0) / SCREEN_WIDTH;
        float stepY = rowDistance * (rayDirY1 - rayDirY0) / SCREEN_WIDTH;

        float floorX = p1.pos.x * posScale + rowDistance * rayDirX0;
        float floorY = p1.pos.y * posScale + rowDistance * rayDirY0;

        for (int x = 0; x < SCREEN_WIDTH; ++x)
        {

            float repeatScale = 1.0f; // how much world space each texture tile covers

            int tx = (int)((floorX / repeatScale) * floorTexture.width) % floorTexture.width;
            int ty = (int)((floorY / repeatScale) * floorTexture.height) % floorTexture.height;

            // Ensure wrapping is safe for negative values
            tx = (tx + floorTexture.width) % floorTexture.width;
            ty = (ty + floorTexture.height) % floorTexture.height;

            int ceilingY = SCREEN_HEIGHT - y;

            // Get the floor and ceiling colors
            Color floorColor = GetImageColor(floorTexture, tx, ty);
            Color ceilingColor = GetImageColor(roofTexture, tx, ty);

            // Set the pixels in the Image data directly (faster than DrawPixel)
            pixels[y * SCREEN_WIDTH + x] = floorColor;
            pixels[ceilingY * SCREEN_WIDTH + x] = ceilingColor;

            floorX += stepX;
            floorY += stepY;
        }
    }

    // After updating the floorImage, update the floorTextureBuffer
    UpdateTexture(*floorTextureBuffer, floorImage->data);

    // Draw the modified floorImage (both floor and ceiling) to the screen
    DrawTexture(*floorTextureBuffer, 0, 0, WHITE); // You can adjust the position here

    for (int c = 0; c < (enemycount + raycount + MAXPROJECTILES); c++)
    {
        if (!allData[c]) // skip null data
            continue;

        switch (isnan(allData[c]->textureOffset)) // Collisions with non wall objects have textureOffset as Nan
        {
        case 1: // Not a wall
        {
            Vec2 enemyPos = allData[c]->position;

            // Vector from player to enemy
            float dx = enemyPos.x - p1.pos.x;
            float dy = enemyPos.y - p1.pos.y;

            // Inverse camera transform
            float invDet = 1.0f / (plane.x * p1.dir.y - p1.dir.x * plane.y);

            float transformX = invDet * (p1.dir.y * dx - p1.dir.x * dy);
            float transformY = invDet * (-plane.y * dx + plane.x * dy);

            if (transformY <= 0)
                continue; // Enemy is behind the player

            // Projected X position on screen
            float enemyScreenX = (SCREEN_WIDTH / 2) * (1 + transformX / transformY);

            Texture2D sprite = allData[c]->texture;

            // Preserve sprite aspect ratio
            float aspectRatio = (float)sprite.width / (float)sprite.height;

            float dist = allData[c]->d;
            float corrected = dist * allData[c]->angle;                 // Correct fisheye effect
            float wallHeight = (TILE_SIZE * SCREEN_HEIGHT) / corrected; // Wall height based on screen size

            // Sprite height scaling factor
            float spritesScale = 24.0;
            float spriteHeight = spritesScale * (SCREEN_HEIGHT / transformY) * 1.8f * ((float)sprite.height / 64.0); // 1.8 = tune to taste
            float spriteWidth = spriteHeight * aspectRatio;

            Rectangle src = {
                0, 0,
                (float)sprite.width,
                (float)sprite.height};

            Rectangle dest = {
                enemyScreenX - spriteWidth / 2,
                SCREEN_HEIGHT / 2 + wallHeight / 2 - spriteHeight,
                spriteWidth,
                spriteHeight};

            DrawTexturePro(sprite, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
            break;
        }
        default: // A wall
        {

            float dist = allData[c]->d;
            float corrected = dist * vectorDot(allData[c]->rayDir, p1.dir); // Correct fisheye effect
            float wallHeight = ((TILE_SIZE * SCREEN_HEIGHT) / corrected);   // Wall height based on screen size

            Texture2D texture = allData[c]->texture;

            float sliceWidth = (float)SCREEN_WIDTH / NUM_RAYS;
            float screenX = allData[c]->id * sliceWidth;

            // --- Draw walls ---
            float texX = allData[c]->textureOffset * texture.width;
            // Source rectangle: a vertical slice of the wall texture
            Rectangle source = {
                texX,
                0,
                1,
                (float)texture.height};

            // Destination rectangle: the scaled vertical slice on screen
            Rectangle destination = {
                screenX, // X on screen
                (SCREEN_HEIGHT / 2.0f) - (wallHeight / 2.0f),
                sliceWidth, // stretches pixels in source retangel to slicewith
                wallHeight};

            DrawTexturePro(texture, source, destination, (Vector2){0, 0}, 0.0f, WHITE);
        }

        break;
        }
    }
    free(allData); // Since we memcpy the only thing stored is pointers to the other pointers and thus the data itself will be freed later
}

void drawMenu(GameState gameState, int frames)
{
    const char *exit = "Exit game [ EXIT ]";
    const char *ret = "Main Menu [ PauseKey ]";
    Vector2 retSize = MeasureTextEx(textFont, ret, textFont.baseSize * 5, 5);

    switch (gameState)
    {
    case MAINMENU:
        // Show main menu
        const char *title = "Schlem on Campus";
        const char *start = "Start Game [ PlayButton ]";

        Vector2 titleSize = MeasureTextEx(jupiter, title, jupiter.baseSize * 10, 10);

        DrawRectangle(SCREEN_WIDTH / 2 - titleSize.x / 2 - 20, SCREEN_HEIGHT / 6 - 20, titleSize.x + 40, SCREEN_HEIGHT / 2, BLACK);

        DrawTextEx(jupiter, title, (Vector2){SCREEN_WIDTH / 2 - titleSize.x / 2, SCREEN_HEIGHT / 6}, jupiter.baseSize * 10, 10, CERISE);
        DrawTextEx(textFont, start, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, start, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 2}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 2 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, CERISE);
        break;
    case GAMEPLAY:
        break;
    case PAUSEMENU:
        // Show pause menu
        const char *resume = "Resume [ PauseKey ]";
        const char *main = "Main Menu [ PlayButton ]";
        Vector2 mainSize = MeasureTextEx(textFont, main, textFont.baseSize * 5, 5);

        DrawRectangle(SCREEN_WIDTH / 2 - mainSize.x / 2 - 20, SCREEN_HEIGHT / 6 - 20, mainSize.x + 40, textFont.baseSize * 15 + 40, BLACK);

        DrawTextEx(textFont, resume, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, resume, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, main, (Vector2){SCREEN_WIDTH / 2 - mainSize.x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, CERISE);
        break;

    case ENDSCREEN:
        // Show end of level screen
        const char *next = "Next level [ PlayButton ]";

        char score[64];

        sprintf(score, "Time: %.1f", (float)frames / (float)FPS_TARGET);

        Vector2 nextSize = MeasureTextEx(textFont, next, textFont.baseSize * 5, 5);

        DrawRectangle(SCREEN_WIDTH / 2 - nextSize.x / 2 - 20, SCREEN_HEIGHT / 6 - 20, nextSize.x + 40, textFont.baseSize * 20 + 40, BLACK);

        DrawTextEx(textFont, next, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, next, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - retSize.x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, score, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, score, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 15 + 5}, textFont.baseSize * 5, 5, CERISE);

        break;
    case DEATHSCREEN:
        // Show death screen
        const char *dead = "YOU DIED";
        const char *retry = "Restart [ PlayButton ]";
        DrawTextEx(textFont, dead, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, dead, textFont.baseSize * 8, 5).x / 2, SCREEN_HEIGHT / 10}, textFont.baseSize * 8, 8, BLACK);
        DrawTextEx(textFont, retry, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, retry, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, BLACK);
        DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - retSize.x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
        DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 15}, textFont.baseSize * 5, 5, BLACK);
        break;
    case THEEND:
        // Show end screen
        const char *won = "YOU'VE WON";
        const char *congrts = "CONGRATULATIONS ON FINISHING THE GAME";
        DrawTextEx(textFont, won, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, won, textFont.baseSize * 8, 5).x / 2, SCREEN_HEIGHT / 10}, textFont.baseSize * 8, 8, CERISE);
        DrawTextEx(textFont, congrts, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, congrts, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, CERISE);
        DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - retSize.x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
        DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 15}, textFont.baseSize * 5, 5, BLACK);
        break;

    default:
        break;
    }
}