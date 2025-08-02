#include "map.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "enemy.h"
#include "sprites.h"
#include "screen.h"

typedef enum
{
    MAINMENU,
    GAMEPLAY,
    PAUSEMENU,
    ENDSCREEN,
    DEATHSCREEN,
    THEEND
} GameState;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Schlem on Campus");
    SetTargetFPS(FPS_TARGET);
    srand(time(NULL));
    SetExitKey(KEY_BACKSPACE); // set close program key, so esc can be used for pause
    ToggleFullscreen();
    HideCursor();

    initSprites();

    Player player = PLAYERINIT;
    GameState gameState = MAINMENU;

    // Load assets
    Map *mp = loadMap(Maps[0]);

    Image floorImage = GenImageColor(SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    Texture2D floorTextureBuffer = LoadTextureFromImage(floorImage);
    Image floorTexture = LoadImageFromTexture(Sprites[MAP_FLOOR]);
    Image roofTexture = LoadImageFromTexture(Sprites[MAP_CEILING]);

    Weapon *weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);

    int currentMap = 0;
    int currentwpn = 0;
    int remainingEnemies = 0;
    const char *exit = "Exit game [ Backspace ]";
    const char *ret = "Main Menu [ Esc ]";

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        CollisionData **hits = multiRayShot(player.pos, player.dir, FOV, mp->numOfWalls, mp->walls, NUM_RAYS); // Gets wall CollisionData

        CollisionData **enemyData = rayShotEnemies(player, FOV, mp, mp->enemies, mp->enemyCount); // Gets enemy CollisionData

        CollisionData **projectileData = rayShotProjectile(player, FOV, mp, mp->projectiles); // Gets projectile CollisionData

        drawScene(&player, (void **)enemyData, mp->enemyCount, (void **)hits, NUM_RAYS, (void **)projectileData, &floorImage, &floorTextureBuffer, floorTexture, roofTexture);

        // Switch between the different states
        switch (gameState)
        {
        case MAINMENU:

            if (IsKeyPressed(KEY_ENTER))
            {
                gameState = GAMEPLAY;
                player.pos = STARTPOS;
                player.dir = (Vec2){0.0, 1.0};
                freeMap(mp);
                mp = loadMap(Maps[currentMap]); // This is very inefficient, but I don't know how to reset a map in a better way
                weapons[2].projectiles = mp->projectiles;
                currentwpn = 0;
            }

            rotate(&player.dir, ROTSPEED / 10);
            // Show main menu
            const char *title = "Schlem on Campus";
            const char *start = "Start Game [ Enter ]";
            DrawTextEx(textFont, title, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, title, textFont.baseSize * 10, 5).x / 2, SCREEN_HEIGHT / 6}, textFont.baseSize * 10, 10, BLACK);
            DrawTextEx(textFont, start, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, start, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 2}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 2 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, BLACK);
            break;

        case GAMEPLAY:
            if (weapons[currentwpn].currentCooldown > 0)
                weapons[currentwpn].currentCooldown--;

            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = PAUSEMENU;
            }

            if (IsKeyDown(KEY_RIGHT))
                rotateRight(&player);
            if (IsKeyDown(KEY_LEFT))
                rotateLeft(&player);
            if (IsKeyDown('W'))
                wishMoveForward(&player);
            if (IsKeyDown('A'))
                wishMoveLeft(&player);
            if (IsKeyDown('S'))
                wishMoveBack(&player);
            if (IsKeyDown('D'))
                wishMoveRight(&player);
            if (IsKeyDown(KEY_SPACE) && weapons[currentwpn].currentCooldown == 0 && weapons[currentwpn].ammo > 0)
                attackEnemy(&weapons[currentwpn], &player, mp);
            if (IsKeyDown('1'))
                currentwpn = 0;
            if (IsKeyDown('2'))
                currentwpn = 1;
            if (IsKeyDown('3'))
                currentwpn = 2;
            if (IsKeyDown('Q'))
                weapons[currentwpn].currentCooldown = 1;
            if (IsKeyDown('E'))
                weapons[currentwpn].currentCooldown = 0;

            executeMovement(&player, mp->walls, mp->numOfWalls);

            remainingEnemies = countHostiles(mp);
            if (remainingEnemies <= 0)
            {
                gameState = ENDSCREEN;
            }
            if (player.hp <= 0)
            {
                gameState = DEATHSCREEN;
            }

            for (int i = 0; i < ENEMY_UPDATES_PER_FRAME; i++)
                updateEnemies(mp->enemies, mp->enemyCount, &player, &weapons[1], &weapons[2], 60, FOV, mp, mp->walls, mp->numOfWalls);

            drawWeapon(weapons, currentwpn);
            for (int i = 0; i < PROJECTILE_UPDATES_PER_FRAME; i++)
                updateProjectiles(mp->projectiles, &player, mp->enemies, mp->enemyCount, &weapons[2], &mp->ppointer);

            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);

            break;

        case PAUSEMENU:

            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = GAMEPLAY;
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset player pos and open main menu
                player.pos = STARTPOS;
                player.dir = (Vec2){0.0, 1.0};
                gameState = MAINMENU;
            }

            // Draw level in background
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);

            // Show pause menu
            const char *resume = "Resume [ Esc ]";
            const char *main = "Main Menu [ Enter ]";
            DrawTextEx(textFont, resume, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, resume, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, main, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, main, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
            break;

        case ENDSCREEN:

            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = MAINMENU;
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                currentMap++; // Advance to next map
                if (currentMap == NUM_MAPS)
                {
                    gameState = THEEND;
                    currentMap = 0;
                    break;
                }

                gameState = GAMEPLAY;
                player.pos = STARTPOS;
                player.dir = (Vec2){0.0, 1.0};

                // Free data before mp changes in order to avoid memory leaks and segmentation faults
                freeCollisionData(hits, NUM_RAYS);
                freeCollisionData(enemyData, mp->enemyCount);
                freeCollisionData(projectileData, MAXPROJECTILES);
                EndDrawing();

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons[2].projectiles = mp->projectiles;
                currentwpn = 0;

                continue; // Only one should be needed
                break;    // Extra just in case
            }

            // Draw level in background
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);

            // Show end of level screen
            const char *next = "Next level [ Enter ]";
            DrawTextEx(textFont, next, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, next, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, ret, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
            break;

        case DEATHSCREEN:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                gameState = MAINMENU;
            }
            if (IsKeyPressed(KEY_ENTER))
            {
                currentMap = 0; // Advance to next map
                gameState = GAMEPLAY;
                player = PLAYERINIT; // Reset player

                // Free data before mp changes in order to avoid memory leaks and segmentation faults
                freeCollisionData(hits, NUM_RAYS);
                freeCollisionData(enemyData, mp->enemyCount);
                freeCollisionData(projectileData, MAXPROJECTILES);
                free(weapons);
                EndDrawing();

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);
                currentwpn = 0;

                continue; // Only one should be needed
                break;    // Extra just in case
            }

            // Draw level in background
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);

            // Show death screen
            const char *dead = "YOU DIED";
            const char *retry = "Restart [ Enter ]";
            DrawTextEx(textFont, dead, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, dead, textFont.baseSize * 8, 5).x / 2, SCREEN_HEIGHT / 10}, textFont.baseSize * 8, 8, BLACK);
            DrawTextEx(textFont, retry, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, retry, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, ret, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 15}, textFont.baseSize * 5, 5, BLACK);
            break;

        case THEEND:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                // Reset the player and show main menu
                gameState = MAINMENU;
                player = PLAYERINIT;
                free(weapons);
                weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);
            }

            // Draw level in background
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);

            // Show end screen
            const char *won = "YOU'VE WON";
            const char *congrts = "CONGRATULATIONS ON FINISHING THE GAME";
            DrawTextEx(textFont, won, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, won, textFont.baseSize * 8, 5).x / 2, SCREEN_HEIGHT / 10}, textFont.baseSize * 8, 8, CERISE);
            DrawTextEx(textFont, congrts, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, congrts, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 5}, textFont.baseSize * 5, 5, CERISE);
            DrawTextEx(textFont, ret, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, ret, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 10}, textFont.baseSize * 5, 5, BLACK);
            DrawTextEx(textFont, exit, (Vector2){SCREEN_WIDTH / 2 - MeasureTextEx(textFont, exit, textFont.baseSize * 5, 5).x / 2, SCREEN_HEIGHT / 6 + textFont.baseSize * 15}, textFont.baseSize * 5, 5, BLACK);
            break;
        default:
            break;
        }
        // Always clean memory
        freeCollisionData(hits, NUM_RAYS);
        freeCollisionData(enemyData, mp->enemyCount);
        freeCollisionData(projectileData, MAXPROJECTILES);
        EndDrawing();
    }

    // --- Shutdown / Cleanup ---

    UnloadImage(floorImage);
    UnloadImage(roofTexture);
    UnloadTexture(floorTextureBuffer);
    UnloadImage(floorTexture);

    freeMap(mp);

    CloseWindow();

    return 0;
}
