#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "controls.h"


int main(void)
{

    InitWindow(S_WIDTH, S_HEIGHT, "RTS GAME");
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

    GameContext ctx =
        {
            .unitList = units,
            .unitCount = &unitCount,
            .maxUnits = maxUnits,
            .warriorData = &warriorData,
            .isDragging = &isDragging,
            .boxStartPoint = &boxStartPoint};

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
        BeginDrawing();
        ClearBackground(WHITE);
        HandleMouseInputs(&ctx);
        for (int i = 0; i < unitCount; i++)
        {
            if (units[i].Draw != NULL)
            {
                units[i].Draw(&units[i]);
            }
        }
        EndDrawing();
    }
    UnloadTexture(warriorData.Idle.texture);
    UnloadTexture(warriorData.Run.texture);
    free(units);
    free(UnitsData);
    CloseWindow();
    return 0;
}
