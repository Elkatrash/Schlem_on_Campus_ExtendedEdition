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
    SetExitKey(KEYBIND_EXIT); // set close program key, so esc can be used for pause
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
    int specialFlag = 0;
    int swapCooldown = 0;
    int mapTimeFrames = 0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        CollisionData **hits = multiRayShot(player.pos, player.dir, FOV, mp->numOfWalls, mp->walls, NUM_RAYS); // Gets wall CollisionData

        CollisionData **enemyData = rayShotEnemies(player, FOV, mp, mp->enemies, mp->enemyCount); // Gets enemy CollisionData

        CollisionData **projectileData = rayShotProjectile(player, FOV, mp, mp->projectiles); // Gets projectile CollisionData

        drawScene(&player, (void **)enemyData, mp->enemyCount, (void **)hits, NUM_RAYS, (void **)projectileData, &floorImage, &floorTextureBuffer, floorTexture, roofTexture);

        drawMenu(gameState);

        if (gameState != MAINMENU)
        {
            drawWeapon(weapons, currentwpn);
            drawHud(&player, &weapons[currentwpn], currentwpn, remainingEnemies, mapTimeFrames);
        }

        // Switch between the different states
        switch (gameState)
        {
        case MAINMENU:

            if (IsKeyPressed(KEYBIND_PLAY))
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

            if (IsKeyPressed(KEYBIND_PAUSE))
            {
                gameState = PAUSEMENU;
            }

            if (IsKeyDown(KEYBIND_TURNRIGHT))
                rotateRight(&player);
            if (IsKeyDown(KEYBIND_TURNLEFT))
                rotateLeft(&player);
            if (IsKeyDown(KEYBIND_MOVEFORWARD))
                wishMoveForward(&player);
            if (IsKeyDown(KEYBIND_MOVELEFT))
                wishMoveLeft(&player);
            if (IsKeyDown(KEYBIND_MOVEBACKWARD))
                wishMoveBack(&player);
            if (IsKeyDown(KEYBIND_MOVERIGHT))
                wishMoveRight(&player);
            if (IsKeyDown(KEYBIND_SHOOT) && weapons[currentwpn].currentCooldown == 0 && weapons[currentwpn].ammo > 0)
                attackEnemy(&weapons[currentwpn], &player, mp);
            if (IsKeyDown(KEYBIND_WEAPON1))
                currentwpn = 0;
            if (IsKeyDown(KEYBIND_WEAPON2))
                currentwpn = 1;
            if (IsKeyDown(KEYBIND_WEAPON3))
                currentwpn = 2;
            if (IsKeyDown(KEYBIND_PREVIOUSWEAPON))
            {
                if (swapCooldown <= 0)
                {
                    currentwpn = (currentwpn >= 1) ? (currentwpn - 1) : (2);
                    swapCooldown = FPS_TARGET / 5;
                }
            }
            if (IsKeyDown(KEYBIND_NEXTWEAPON))
            {
                if (swapCooldown <= 0)
                {
                    currentwpn = (currentwpn <= 1 && swapCooldown <= 0) ? (currentwpn + 1) : (0);
                    swapCooldown = FPS_TARGET / 5;
                }
            }
            swapCooldown--;

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
                updateEnemies(&player, &weapons[1], &weapons[2], FPS_TARGET, FOV, mp);

            for (int i = 0; i < PROJECTILE_UPDATES_PER_FRAME; i++)
                updateProjectiles(&player, &weapons[2], mp);

            mapTimeFrames++;
            break;

        case PAUSEMENU:

            if (IsKeyPressed(KEYBIND_PAUSE))
            {
                gameState = GAMEPLAY;
            }
            if (IsKeyPressed(KEYBIND_PLAY))
            {
                // Reset player pos and open main menu
                player.pos = STARTPOS;
                player.dir = (Vec2){0.0, 1.0};
                gameState = MAINMENU;
            }

            break;

        case ENDSCREEN:

            if (IsKeyPressed(KEYBIND_PAUSE))
            {
                gameState = MAINMENU;
            }
            if (IsKeyPressed(KEYBIND_PLAY))
            {
                currentMap++; // Advance to next map
                mapTimeFrames = 0;
                if (currentMap == NUM_MAPS)
                {
                    gameState = THEEND;
                    currentMap = 0;
                    break;
                }

                gameState = GAMEPLAY;
                player.pos = STARTPOS;
                player.dir = (Vec2){0.0, 1.0};

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons[2].projectiles = mp->projectiles;
                currentwpn = 0;
                specialFlag = 1;
                break; // Extra just in case
            }

            break;

        case DEATHSCREEN:
            if (IsKeyPressed(KEYBIND_PLAY) || IsKeyPressed(KEYBIND_PAUSE))
            {
                currentMap = 0; // Advance to next map
                mapTimeFrames = 0;

                gameState = (IsKeyPressed(KEYBIND_PAUSE)) ? MAINMENU : GAMEPLAY;
                player = PLAYERINIT; // Reset player

                free(weapons);
                EndDrawing();

                freeMap(mp);                    // Unload old map
                mp = loadMap(Maps[currentMap]); // load next Map
                weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);
                currentwpn = 0;

                break; // Extra just in case
            }

            break;

        case THEEND:
            if (IsKeyPressed(KEYBIND_PAUSE))
            {
                // Reset the player and show main menu
                gameState = MAINMENU;
                player = PLAYERINIT;
                free(weapons);
                weapons = getWeapons(SCREEN_WIDTH, SCREEN_HEIGHT, mp->projectiles);
                mapTimeFrames = 0;
            }

            break;
        default:
            break;
        }

        // Always clean memory
        freeCollisionData(hits, NUM_RAYS);
        (!specialFlag) ? freeCollisionData(enemyData, mp->enemyCount) : (specialFlag = 0);
        freeCollisionData(projectileData, MAXPROJECTILES);
        EndDrawing();
    }

    // --- Shutdown / Cleanup ---

    UnloadImage(floorImage);
    UnloadImage(roofTexture);
    UnloadTexture(floorTextureBuffer);
    UnloadImage(floorTexture);

    freeMap(mp);
    destroySprites();

    CloseWindow();

    return 0;
}
