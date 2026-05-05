#include "controls.h"
#include "./util/offsets.h"
#include "map/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void HandleMouseInputs(GameContext *ctx)
{
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), *ctx->cam);
        
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            if (*(ctx->unitCount) < ctx->maxUnits)
            {
                ctx->unitList[*(ctx->unitCount)] = Create_Warrior(
                    ctx->warriorData, mouseWorld, ctx->unitCount, ctx->map);
                (*(ctx->unitCount))++;
                printf("SPAWNED\n");
            }
        }
        else
        {
            printf("RIGHT CLICK MOVE\n");
            int targetChunkX = (int)(mouseWorld.x / CHUNK_SIZE_PX);
            int targetChunkY = (int)(mouseWorld.y / CHUNK_SIZE_PX);
            
            if (targetChunkX < 0) targetChunkX = 0;
            if (targetChunkY < 0) targetChunkY = 0;
            if (targetChunkX >= CHUNKS_PER_COLUMN) targetChunkX = CHUNKS_PER_COLUMN - 1;
            if (targetChunkY >= CHUNKS_PER_ROW) targetChunkY = CHUNKS_PER_ROW - 1;
            
            int targetChunkId = targetChunkY * CHUNKS_PER_COLUMN + targetChunkX;
            
            for (int i = 0; i < *(ctx->unitCount); i++)
            {
                if (ctx->unitList[i].isSelected)
                {
                    printf("UNIT %d IS SELECTED, RUNNING A*\n", i);
                    
                    int startChunkX = (int)(ctx->unitList[i].pos.x / CHUNK_SIZE_PX);
                    int startChunkY = (int)(ctx->unitList[i].pos.y / CHUNK_SIZE_PX);
                    
                    if (startChunkX < 0) startChunkX = 0;
                    if (startChunkY < 0) startChunkY = 0;
                    if (startChunkX >= CHUNKS_PER_COLUMN) startChunkX = CHUNKS_PER_COLUMN - 1;
                    if (startChunkY >= CHUNKS_PER_ROW) startChunkY = CHUNKS_PER_ROW - 1;
                    
                    int startChunkId = startChunkY * CHUNKS_PER_COLUMN + startChunkX;
                    
                    printf("CALLING ASTAR: start=%d goal=%d\n", startChunkId, targetChunkId);
                    
                    int pathLength = 0;
                    int* resultPath = AStar_FindPath(ctx->astar, startChunkId, targetChunkId, &pathLength);
                    
                    if (resultPath != NULL && pathLength > 0)
                    {
                        printf("ASTAR RETURNED path, len=%d\n", pathLength);

                        // --- ΔΙΟΡΘΩΣΗ: Deep Copy του Path ---
                        if (ctx->unitList[i].path != NULL) {
                            free(ctx->unitList[i].path);
                            ctx->unitList[i].path = NULL;
                        }

                        ctx->unitList[i].path = malloc(sizeof(int) * pathLength);
                        memcpy(ctx->unitList[i].path, resultPath, sizeof(int) * pathLength);
                        ctx->unitList[i].pathLength = pathLength;
                        ctx->unitList[i].finalTarget = mouseWorld;
                        
                        if (pathLength > 1)
                        {
                            // Ξεκινάμε από το index 0 (τρέχον chunk)
                            ctx->unitList[i].currentPathIndex = 0; 
                            
                            // --- ΔΙΟΡΘΩΣΗ: Χρήση Πύλης (Waypoint) αντί για το κέντρο του Chunk ---
                            ctx->unitList[i].targetPos = GetNextWaypoint(ctx->map, resultPath[0], resultPath[1]);
                        }
                        else
                        {
                            // Αν είμαστε ήδη στο ίδιο chunk με τον στόχο
                            ctx->unitList[i].currentPathIndex = 0;
                            ctx->unitList[i].targetPos = mouseWorld;
                        }
                        
                        ctx->unitList[i].currentState = STATE_RUN;
                    }
                }
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        *(ctx->isDragging) = true;
        *(ctx->boxStartPoint) = GetScreenToWorld2D(GetMousePosition(), *ctx->cam);
    }

    if (*(ctx->isDragging))
    {
        Vector2 currentMousePos = GetScreenToWorld2D(GetMousePosition(), *ctx->cam);
        Rectangle selectionBox = {
            .x = fminf(ctx->boxStartPoint->x, currentMousePos.x),
            .y = fminf(ctx->boxStartPoint->y, currentMousePos.y),
            .width = fabsf(currentMousePos.x - ctx->boxStartPoint->x),
            .height = fabsf(currentMousePos.y - ctx->boxStartPoint->y)
        };
        
        DrawRectangleRec(selectionBox, Fade(SKYBLUE, 0.3f));
        DrawRectangleLinesEx(selectionBox, 1.0f, BLUE);

        for (int i = 0; i < *(ctx->unitCount); i++)
        {
            float fw = (float)ctx->unitList[i].data->Idle.texture.width / ctx->unitList[i].data->Idle.frameNum;
            float fh = (float)ctx->unitList[i].data->Idle.texture.height;
            
            // Επιλογή με βάση τα πόδια/βάση του unit
            Vector2 feet = {ctx->unitList[i].pos.x + fw/2.0f, ctx->unitList[i].pos.y + fh*0.9f};

            if (selectionBox.width > 2 || selectionBox.height > 2)
            {
                ctx->unitList[i].isSelected = CheckCollisionPointRec(feet, selectionBox);
            }
            else
            {
                Vector2 mp = GetScreenToWorld2D(GetMousePosition(), *ctx->cam);
                Rectangle unitRect = {ctx->unitList[i].pos.x, ctx->unitList[i].pos.y, fw, fh};
                ctx->unitList[i].isSelected = CheckCollisionPointRec(mp, unitRect);
            }
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            *(ctx->isDragging) = false;
        }
    }
}

void CameraMovment(Camera2D *cam)
{
    float speed = 500.0f * GetFrameTime();
    if (IsKeyDown(KEY_W)) cam->target.y -= speed;
    if (IsKeyDown(KEY_S)) cam->target.y += speed;
    if (IsKeyDown(KEY_A)) cam->target.x -= speed;
    if (IsKeyDown(KEY_D)) cam->target.x += speed;
    
    float wheel = GetMouseWheelMove();
    cam->zoom += wheel * 0.1f;
    if (cam->zoom < 0.05f) cam->zoom = 0.05f; // Λίγο πιο λογικό zoom limit
}