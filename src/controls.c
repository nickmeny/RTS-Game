#include "controls.h"
#include "./util/offsets.h"
/// @brief This function Handles the mouse control
/// @param data (UnitData*)  the unit data of the unit
/// @param unitList (Unit*) the list of the spawn units
/// @param count (int*) the counter of the unitsList
/// @param max (int) the maximoum cupacity of the unitList
/// @param isDragging (bool*)
/// @param boxStartPoint (Vector2*)
void HandleMouseInputs(GameContext *ctx)
{
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        Vector2 mouseTarget = GetMousePosition();
        // Point targetTile = {(int)mouseTarget.x / 30, (int)mouseTarget.y / 30};

        for (int i = 0; i < *(ctx->unitCount); i++)
        {
            if (ctx->unitList[i].isSelected)
            {
                ctx->unitList[i].targetPos = mouseTarget;
                ctx->unitList[i].currentState = STATE_RUN;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
    {
        if (*(ctx->unitCount) < ctx->maxUnits)
        {
            Vector2 pos = GetMousePosition();
            // This section is only for the visuals to make the asset to spawn in the tip of the mouse
            Vector2 adjustedPos = {pos.x - (float)ctx->warriorData->body.bodyCenter[0].x, pos.y - (float)ctx->warriorData->body.bodyCenter[0].y};

            // Create and save the warrior in the list
            ctx->unitList[*(ctx->unitCount)] = Create_Warrior(ctx->warriorData, adjustedPos, ctx->unitCount);
            (*(ctx->unitCount))++;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))

    { // Enable the Selection
        *(ctx->isDragging) = true;
        *(ctx->boxStartPoint) = GetMousePosition();
        // if (!IsKeyDown(KEY_LEFT_SHIFT)) {
        //     for(int i = 0; i < *count; i++) unitList[i].isSelected = false;
        // }
    }

    if (*(ctx->isDragging))
    {

        Vector2 currentMousePos = GetMousePosition();

        // Draw the selection Rec
        Rectangle selectionBox = {
            .x = fminf(ctx->boxStartPoint->x, currentMousePos.x),
            .y = fminf(ctx->boxStartPoint->y, currentMousePos.y),
            .width = fabsf(currentMousePos.x - ctx->boxStartPoint->x),
            .height = fabsf(currentMousePos.y - ctx->boxStartPoint->y)};

        DrawRectangleRec(selectionBox, Fade(SKYBLUE, 0.3f));
        DrawRectangleLinesEx(selectionBox, 1.0f, BLUE);

        for (int i = 0; i < *(ctx->unitCount); i++)
        {
            // Calculate the unit's "center feet" position
            float fw = (float)ctx->unitList[i].data->Idle.texture.width / ctx->unitList[i].data->Idle.frameNum;
            float fh = (float)ctx->unitList[i].data->Idle.texture.height;
            Vector2 feet = {ctx->unitList[i].pos.x + (fw / 2), ctx->unitList[i].pos.y + (fh * 0.9f)};

            // If the box is just a tiny click, select one unit.
            // If it's a big box, select everything inside.
            if (selectionBox.width > 2 || selectionBox.height > 2)
            {
                ctx->unitList[i].isSelected = CheckCollisionPointRec(feet, selectionBox);
            }
            else
            {
                ctx->unitList[i].isSelected = CheckCollisionPointRec(GetMousePosition(), (Rectangle){ctx->unitList[i].pos.x, ctx->unitList[i].pos.y, fw, fh});
            }
            printf("%d\n", ctx->unitList[i].isSelected ? ctx->unitList[i].id : 300);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            *(ctx->isDragging) = false;
        }
    }
}