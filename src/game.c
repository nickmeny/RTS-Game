#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "controls.h"
#include "./map/map.h"
int main(void)
{

    InitWindow(S_WIDTH, S_HEIGHT, "RTS GAME");
    SetTargetFPS(60);
    Vector2 boxStartPoint = {0, 0};
    bool isDragging = false;
    UnitData *UnitsData = calloc(3, sizeof(UnitData));
    UnitData warriorData = Init_Warrior();
    UnitsData[0] = warriorData;
    Camera2D camera = {0};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){S_WIDTH/2, S_HEIGHT/2};
    camera.rotation = 0.0f;
    camera.zoom = 0.1f;
    int maxUnits = 500;
    Unit *units = (Unit *)calloc(maxUnits, sizeof(Unit));

    if (units == NULL)
    {
        return 1;
    }

    int unitCount = 0;

    Map map = create_map();

    GameContext ctx =
        {
            .unitList = units,
            .unitCount = &unitCount,
            .maxUnits = maxUnits,
            .warriorData = &warriorData,
            .isDragging = &isDragging,
            .boxStartPoint = &boxStartPoint,
            .cam = &camera,
            .map = &map,
            .astar = AStar_Create(&map)
        };

    while (!WindowShouldClose())
    {
        for (int i = 0; i < unitCount; i++)
        {
            if (units[i].Update != NULL)
            {
                units[i].Update(&units[i]);
            }
            // Run collision here too so they don't overlap while walking!
            UnitCollision(&units[i], units, unitCount);
        }
        CameraMovment(&camera);
        BeginDrawing();
        ClearBackground(WHITE);
        BeginMode2D(camera);
        DrawChunksFromMemory(&map,&camera);
        HandleMouseInputs(&ctx);
        for (int i = 0; i < unitCount; i++)
        {
            if (units[i].Draw != NULL)
            {
                units[i].Draw(&units[i]);
            }
        }

        EndMode2D();
        EndDrawing();

    }
    UnloadTexture(warriorData.Idle.texture);
    UnloadTexture(warriorData.Run.texture);

    
    free(warriorData.body.bodyCenter);
    free(warriorData.body.frameGeometry);
    free(warriorData.body.bodyGeometry);
    free(units);
    free(UnitsData);
    free_map(map);
    CloseWindow();
    return 0;
}
