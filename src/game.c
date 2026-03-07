#include <stdio.h>
#include <stdlib.h>
#include "game.h"


int main(void)
{

    InitWindow(S_WIDTH, S_HEIGHT, "RTS GAME");
    int map_size[2] = {S_HEIGHT / 30, S_WIDTH / 30};
    SetTargetFPS(60);
    Vector2 boxStartPoint = {0, 0};
    bool isDragging = false;
    UnitData *UnitsData = calloc(3, sizeof(UnitData));
    UnitData warriorData = Init_Warrior();
    UnitsData[0] = warriorData;

    int maxUnits = 500;
    Unit *units = (Unit *)calloc(maxUnits, sizeof(Unit));

    if (units == NULL)
    {
        return 1;
    }

    int unitCount = 0;
    bool **map = map_creation(map_size);
    Queue q;
    init_queue(&q, map_size[1] * map_size[0]);
    int **costs = (int **)malloc(map_size[0] * sizeof(int *));
    for (int i = 0; i < map_size[0]; i++)
    {
        costs[i] = (int *)malloc(map_size[1] * sizeof(int));
    }

    GameContext ctx =
        {
            .unitList = units,
            .unitCount = &unitCount,
            .maxUnits = maxUnits,
            .warriorData = &warriorData,
            .flowQueue = &q,
            .costs = costs,
            .map = map,
            .mapW = map_size[1],
            .mapH = map_size[0],
            .isDragging = &isDragging,
            .boxStartPoint = &boxStartPoint};

    while (!WindowShouldClose())
    {
        for (int i = 0; i < unitCount; i++)
        {
            if (units[i].Update != NULL)
            {
                units[i].Update(&units[i]);
                units[i].flowfield = costs;
            }
            // Run collision here too so they don't overlap while walking!
            UnitCollision(&units[i], units, unitCount);
        }
        BeginDrawing();
        ClearBackground(WHITE);
        show_map(map, map_size);
        HandleMouseInputs(&ctx);
        for (int i = 0; i < unitCount; i++)
        {
            if (units[i].Draw != NULL)
            {
                units[i].Draw(&units[i]);
            }
        }
        if (IsKeyDown(KEY_D))
        { // Πάτα το 'D' για να τα δεις
            for (int y = 1; y < map_size[0] - 1; y++)
            {
                for (int x = 1; x < map_size[1] - 1; x++)
                {
                    if (costs[y][x] < 999)
                    {
                        Vector2 dir = get_flow_direction(costs, x, y, map_size[0], map_size[1]);
                        Vector2 start = {x * 30 + 15, y * 30 + 15};             // Κέντρο του tile
                        Vector2 end = Vector2Add(start, Vector2Scale(dir, 10)); // Βέλος 10 pixels
                        DrawLineV(start, end, RED);
                        DrawCircleV(end, 2, RED);
                    }
                }
            }
        }
        EndDrawing();
    }
    UnloadTexture(warriorData.Idle.texture);
    UnloadTexture(warriorData.Run.texture);
    free(units);
    free_queue(&q);
    free_table_malloc((void **)map, map_size[0]);
    free_table_malloc((void*)costs,map_size[0]);
    free(UnitsData);
    CloseWindow();
    return 0;
}
