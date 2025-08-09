#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "raylib.h"
#include "map.h"
#include "movement.h"
#include "raycast.h"
#include "enemy.h"
#include "sprites.h"

const char *Maps[] = {
    "Data/Maps/map1.csv",
    "Data/Maps/map2.csv",
    "Data/Maps/Map3.csv",
    "Data/Maps/Map4.csv"};

FILE *newMap(const char *filename)
{
    return fopen(filename, "w"); // It's not the size of the function that matters
}

int addShape(FILE *map, Vec2 *corners, const char *texture, int cornercount, int closed)
{
    if (!map)                                   // if the file isn't opened properly
        return 0;                               // Fail
    for (int i = 0; i < (cornercount - 1); i++) // Move through all corners in order and create walls as you go along
    {
        fprintf(map, "%f,%f,%f,%f,%s\n", corners[i].x, corners[i].y, corners[i + 1].x, corners[i + 1].y, texture);
    }
    if (closed) // if the shape is closed make a wall between the start and end
    {
        fprintf(map, "%f,%f,%f,%f,%s\n", corners[cornercount - 1].x, corners[cornercount - 1].y, corners[0].x, corners[0].y, texture);
    }
    return 1; // Success
}

int addEnemy(FILE *map, Vec2 pos, int id, EnemyType type)
{

    if (!map)                                              // if the file isn't opened properly
        return 0;                                          // Fail
    fprintf(map, "%f,%f,%d,%d\n", pos.x, pos.y, id, type); // write enemy properties to file
    return 1;                                              // success!
}

Map *loadMap(const char *filename)
{

    // Opening file
    FILE *mfile = fopen(filename, "r");
    if (!mfile)
    {
        printf("Could not open file");
        return NULL;
    }
    // Allocating memory
    Map *result = malloc(sizeof(Map));
    if (!result)
    {
        printf("Malloc error");
        return NULL;
    }

    char buffer[128];
    // Read the format line of the file. I.E the first line
    if (!fgets(buffer, sizeof(buffer), mfile))
    {
        printf("Could not read format of file");
        fclose(mfile);
        free(result);
        return NULL;
    }
    int nwalls, nenemy;
    sscanf(buffer, "%d,%d", &nwalls, &nenemy); // decipher the format
    result->numOfWalls = nwalls;
    result->enemyCount = nenemy;

    result->walls = malloc(sizeof(Wall) * nwalls); // Allocate memory for walls according to format
    if (!result->walls)
    {
        printf("Malloc error");
        fclose(mfile);
        free(result);
        return NULL;
    }
    if (nenemy)
    {
        result->enemies = malloc(sizeof(Enemy) * nenemy); // Allocate memory for enemies according to format
        if (!result->enemies)
        {
            printf("Malloc error");
            fclose(mfile);
            free(result->walls);
            free(result);
            return NULL;
        }
    }
    else
    {
        result->enemies = NULL;
    }
    // read the first lines as walls. Loads coordinates and textures
    for (int i = 0; i < nwalls && fgets(buffer, sizeof(buffer), mfile); i++)
    {
        int wallSprite;
        // Read the wall data
        sscanf(buffer, "%f,%f,%f,%f,%d", &result->walls[i].start.x, &result->walls[i].start.y, &result->walls[i].stop.x, &result->walls[i].stop.y, &wallSprite);
        result->walls[i].texture = Sprites[wallSprite];
    }
    // Read the remaining lines as enemies. Loads attributes and sets som default attributes and also loads sprite.
    for (int i = 0; i < nenemy && fgets(buffer, sizeof(buffer), mfile) && nenemy; i++)
    {
        int type;
        int id;
        float x;
        float y;
        // Read pos, id, and type from file
        sscanf(buffer, "%f,%f,%d,%d", &x, &y, &id, &type);

        result->enemies[i] = EnemyPresets[type];
        result->enemies[i].id = id;
        result->enemies[i].pos = (Vec2){x, y};
        result->enemies[i].hitRadius = (Sprites[result->enemies[i].sprite].width * 16) / 64;
        result->enemies[i].baseCoolDown *= ((float)ENEMY_UPDATES_PER_FRAME / (float)nenemy);
        result->enemies[i].acceleration *= ((float)nenemy / (float)ENEMY_UPDATES_PER_FRAME);
        result->enemies[i].maxSpeed *= ((float)nenemy / (float)ENEMY_UPDATES_PER_FRAME);
    }

    result->projectiles = malloc(MAXPROJECTILES * sizeof(Enemy *));
    for (int i = 0; i < MAXPROJECTILES; i++)
        result->projectiles[i] = NULL;
    result->ppointer = 0;
    fclose(mfile); // close file
    return result; // return map
}

void freeMap(Map *m)
{
    // If memory allocated -> free memory allocated.
    if (m->walls)
        free(m->walls);
    if (m->enemies)
        free(m->enemies);
    if (m->projectiles)
    {
        for (int i = 0; i < MAXPROJECTILES; i++)
        {
            if (m->projectiles[i])
            {
                free(m->projectiles[i]);
            }
        }
        free(m->projectiles);
    }

    free(m);
}