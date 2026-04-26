#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../engine.h"
#include "map.h"

#define MAX_NEIGHBORS 8

Map create_map() {
    Map map;
    map.chunks = malloc(sizeof(Chunk) * CHUNKS_NUMBER);
    
    // Αρχικοποίηση
    for (int i = 0; i < CHUNKS_PER_ROW; i++) {
        for (int j = 0; j < CHUNKS_PER_COLUMN; j++) {
            int index = i * CHUNKS_PER_COLUMN + j;
            map.chunks[index].id = index;
            map.chunks[index].gridX = j;
            map.chunks[index].gridY = i;
            map.chunks[index].walkable = true;
            map.chunks[index].neighbors = malloc(sizeof(Chunk*) * MAX_NEIGHBORS);
            map.chunks[index].neighborCount = 0;
            map.chunks[index].tiles = NULL;      // προς το παρόν
            map.chunks[index].entrance_tiles = NULL;
        }
    }
    
    // Σύνδεση γειτόνων
    for (int y = 0; y < CHUNKS_PER_ROW; y++) {
        for (int x = 0; x < CHUNKS_PER_COLUMN; x++) {
            Chunk* current = &map.chunks[y * CHUNKS_PER_COLUMN + x];
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < CHUNKS_PER_COLUMN && 
                        ny >= 0 && ny < CHUNKS_PER_ROW) {
                        Chunk* neighbor = &map.chunks[ny * CHUNKS_PER_COLUMN + nx];
                        current->neighbors[current->neighborCount++] = neighbor;
                    }
                }
            }
        }
    }
    
    return map;
}
void DrawChunksFromMemory(Map* graph, Camera2D* cam) {
    // Υπολόγισε ποια chunks είναι ορατά
    float viewWidth  = S_WIDTH  / cam->zoom;
    float viewHeight = S_HEIGHT / cam->zoom;
    
    float viewLeft   = cam->target.x - viewWidth/2;
    float viewRight  = cam->target.x + viewWidth/2;
    float viewTop    = cam->target.y - viewHeight/2;
    float viewBottom = cam->target.y + viewHeight/2;
    
    float chunkPx = CHUNK_SIZE * TILE_SIZE;  // 1024
    
    int startX = (int)(viewLeft / chunkPx) - 1;
    int startY = (int)(viewTop / chunkPx) - 1;
    int endX   = (int)(viewRight / chunkPx) + 1;
    int endY   = (int)(viewBottom / chunkPx) + 1;
    
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > CHUNKS_PER_COLUMN) endX = CHUNKS_PER_COLUMN;
    if (endY > CHUNKS_PER_ROW) endY = CHUNKS_PER_ROW;
    
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            int index = y * CHUNKS_PER_COLUMN + x;
            Chunk* chunk = &graph->chunks[index];
            
            float px = x * chunkPx;
            float py = y * chunkPx;
            
            Color col = chunk->walkable ? (Color){0,100,0,100} : (Color){50,50,50,100};
            DrawRectangle(px, py, chunkPx, chunkPx, col);
            DrawRectangleLines(px, py, chunkPx, chunkPx, GRAY);
            
            // Γραμμές προς γείτονες
            for (int j = 0; j < chunk->neighborCount; j++) {
                Chunk* neighbor = chunk->neighbors[j];
                float cx = px + chunkPx/2;
                float cy = py + chunkPx/2;
                float nx = neighbor->gridX * chunkPx + chunkPx/2;
                float ny = neighbor->gridY * chunkPx + chunkPx/2;
                DrawLine(cx, cy, nx, ny, YELLOW);
            }
            
            DrawText(TextFormat("%d", chunk->id), px + 4, py + 4, 10, WHITE);
        }
    }
}

void free_map(Map map) {
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        free(map.chunks[i].neighbors);
    }
    free(map.chunks);
}