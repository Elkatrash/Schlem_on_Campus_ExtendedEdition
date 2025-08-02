#include "map.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"
#include "sprites.h"
#include "screen.h"

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

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        CollisionData **hits = multiRayShot(player.pos, player.dir, FOV, mp->numOfWalls, mp->walls, NUM_RAYS); // Gets wall CollisionData

        CollisionData **enemyData = rayShotEnemies(player, FOV, mp, mp->enemies, mp->enemyCount); // Gets enemy CollisionData

        CollisionData **projectileData = rayShotProjectile(player, FOV, mp, mp->projectiles); // Gets projectile CollisionData

        drawScene(&player, (void **)enemyData, mp->enemyCount, (void **)hits, NUM_RAYS, (void **)projectileData, &floorImage, &floorTextureBuffer, floorTexture, roofTexture);

        drawMenu(gameState);

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

            for (int i = 0; i < PROJECTILE_UPDATES_PER_FRAME; i++)
                updateProjectiles(mp->projectiles, &player, mp->enemies, mp->enemyCount, &weapons[2], &mp->ppointer);

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
                /*
                freeCollisionData(hits, NUM_RAYS);
                freeCollisionData(enemyData, mp->enemyCount);
                freeCollisionData(projectileData, MAXPROJECTILES);
                EndDrawing();*/

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons[2].projectiles = mp->projectiles;
                currentwpn = 0;

                // continue; // Only one should be needed
                break; // Extra just in case
            }

            break;

        case DEATHSCREEN:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE))
            {
                currentMap = 0; // Advance to next map

                gameState = (IsKeyPressed(KEY_ESCAPE)) ? MAINMENU : GAMEPLAY;
                player = PLAYERINIT; // Reset player

                // Free data before mp changes in order to avoid memory leaks and segmentation faults
                /*
                freeCollisionData(hits, NUM_RAYS);
                freeCollisionData(enemyData, mp->enemyCount);
                freeCollisionData(projectileData, MAXPROJECTILES);*/
                free(weapons);
                EndDrawing();

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);
                currentwpn = 0;

                // continue; // Only one should be needed
                break; // Extra just in case
            }

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

            break;
        default:
            break;
        }
        if (gameState != MAINMENU)
        {
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies);
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
