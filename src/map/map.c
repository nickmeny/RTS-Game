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

            map.chunks[index].entrance_count=0;
            map.chunks[index].entrance_points=NULL;
            map.chunks[index].inner_distances = NULL;
            map.chunks[index].tiles = malloc(sizeof(Tile*)*CHUNK_SIZE);
            for(int r =0;r<CHUNK_SIZE;r++)
            {
                map.chunks[index].tiles[r] = malloc(sizeof(Tile)*CHUNK_SIZE);
                for(int col=0;col<CHUNK_SIZE;col++)
                {
                    map.chunks[index].tiles[r][col].is_walkable=true;
                    map.chunks[index].tiles[r][col].pos = (Vector2){(j*CHUNK_SIZE+col)*TILE_SIZE,(i*CHUNK_SIZE+r)*TILE_SIZE};
                }
            }
            map.chunks[index].bounds = (Rectangle){ 
                j * CHUNK_SIZE * TILE_SIZE, 
                i * CHUNK_SIZE * TILE_SIZE, 
                CHUNK_SIZE * TILE_SIZE, 
                CHUNK_SIZE * TILE_SIZE 
            };
        }
    }
    
    // ========== ΜΗ-WALKABLE CHUNKS ==========
    
    // Μια "λίμνη" 3x3 στο κέντρο του map
    int centerX = CHUNKS_PER_COLUMN / 2;
    int centerY = CHUNKS_PER_ROW / 2;
    for (int y = centerY - 1; y <= centerY + 1; y++) {
        for (int x = centerX - 1; x <= centerX + 1; x++) {
            int idx = y * CHUNKS_PER_COLUMN + x;
            map.chunks[idx].walkable = false;
        }
    }
    
    // Ένα διαγώνιο "τείχος"
    for (int i = 10; i < 20; i++) {
        int idx = i * CHUNKS_PER_COLUMN + i;
        if (idx < CHUNKS_NUMBER) {
            map.chunks[idx].walkable = false;
        }
    }
    
    // Μερικά σκόρπια εμπόδια
    for (int i = 0; i < 50; i++) {
        int rx = 5 + (i * 7) % (CHUNKS_PER_COLUMN - 10);
        int ry = 5 + (i * 13) % (CHUNKS_PER_ROW - 10);
        int idx = ry * CHUNKS_PER_COLUMN + rx;
        map.chunks[idx].walkable = false;
    }
    
    // ========== ΣΥΝΔΕΣΗ ΓΕΙΤΟΝΩΝ ==========
    for (int y = 0; y < CHUNKS_PER_ROW; y++) {
        for (int x = 0; x < CHUNKS_PER_COLUMN; x++) {
            Chunk* current = &map.chunks[y * CHUNKS_PER_COLUMN + x];
            
            // Αν δεν είναι walkable, δεν το συνδέουμε
            if (!current->walkable) continue;
            
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < CHUNKS_PER_COLUMN && 
                        ny >= 0 && ny < CHUNKS_PER_ROW) {
                        Chunk* neighbor = &map.chunks[ny * CHUNKS_PER_COLUMN + nx];
                        
                        // Σύνδεσε μόνο με walkable γείτονες
                        if (neighbor->walkable) {
                            current->neighbors[current->neighborCount++] = neighbor;
                        }
                    }
                }
            }
        }
    }
    
    return map;
}

void DrawChunksFromMemory(Map* graph, Camera2D* cam) {
    float viewWidth  = S_WIDTH  / cam->zoom;
    float viewHeight = S_HEIGHT / cam->zoom;
    
    float viewLeft   = cam->target.x - viewWidth/2;
    float viewRight  = cam->target.x + viewWidth/2;
    float viewTop    = cam->target.y - viewHeight/2;
    float viewBottom = cam->target.y + viewHeight/2;
    
    float chunkPx = CHUNK_SIZE * TILE_SIZE;
    
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
            
            Color col = chunk->walkable ? (Color){0,100,0,100} : (Color){80,30,30,150};
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
            if (chunk->walkable) {
                for (int r = 0; r < CHUNK_SIZE; r++) {
                    for (int c = 0; c < CHUNK_SIZE; c++) {
                        if (!chunk->tiles[r][c].is_walkable) {
                            DrawRectangleV(chunk->tiles[r][c].pos, (Vector2){TILE_SIZE, TILE_SIZE}, BLACK);
                        }
                    }
                }
            }
            // Μέσα στην DrawChunksFromMemory, εκεί που σχεδιάζεις το chunk
            for (int e = 0; e < chunk->entrance_count; e++) {
                DrawCircleV(chunk->entrance_points[e], 5, BLUE); // Μπλε κύκλοι για τις πύλες
            }
        }
    }
}
void FinalizeMapPathfinding(Map *map) {
    printf("HPA*: Building entrances...\n");
    build_entrances(map);
    
    printf("HPA*: Computing intra-edges...\n");
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        compute_intra_edges(&map->chunks[i]);
    }
    printf("HPA*: Map is ready!\n");
}


void compute_intra_edges(Chunk *chunk) {
    if (chunk->entrance_count == 0 || !chunk->walkable) return;

    // Δυναμικό allocation για τον πίνακα αποστάσεων (flattened 2D)
    chunk->inner_distances = malloc(sizeof(float) * chunk->entrance_count * chunk->entrance_count);

    for (int i = 0; i < chunk->entrance_count; i++) {
        for (int j = 0; j < chunk->entrance_count; j++) {
            if (i == j) {
                chunk->inner_distances[i * chunk->entrance_count + j] = 0.0f;
                continue;
            }

            // Εδώ καλούμε έναν A* που περιορίζεται ΜΟΝΟ μέσα στο chunk
            // Θα του δώσουμε το chunk, την πύλη εκκίνησης και την πύλη στόχο
            float dist = local_tile_astar(chunk, chunk->entrance_points[i], chunk->entrance_points[j]);
            
            chunk->inner_distances[i * chunk->entrance_count + j] = dist;
        }
    }
}

void free_map(Map map) {
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        free(map.chunks[i].neighbors);
        if(map.chunks[i].tiles!=NULL)
        {
            for (int r = 0; r < CHUNK_SIZE; r++) {
                free(map.chunks[i].tiles[r]);
            }
            free(map.chunks[i].tiles);
        }
        if (map.chunks[i].entrance_points != NULL) free(map.chunks[i].entrance_points);
        if (map.chunks[i].inner_distances != NULL) free(map.chunks[i].inner_distances);
    }
    free(map.chunks);
}

void build_entrances(Map *map) {
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        Chunk *c = &map->chunks[i];
        if (!c->walkable) continue;

        // Προσωρινός πίνακας για να αποθηκεύσουμε τις πύλες πριν τις κάνουμε malloc
        Vector2 temp_pts[16]; 
        int count = 0;

        // Σκανάρουμε τις 4 πλευρές του chunk (Πάνω, Κάτω, Αριστερά, Δεξιά)
        // Για απλότητα, βάζουμε μια πύλη στη μέση κάθε πλευράς αν είναι walkable
        
        // 1. Πάνω πλευρά (r = 0)
        if (c->gridY > 0 && map->chunks[i - CHUNKS_PER_COLUMN].walkable) {
            if (c->tiles[0][CHUNK_SIZE/2].is_walkable) {
                temp_pts[count++] = c->tiles[0][CHUNK_SIZE/2].pos;
            }
        }
        // 2. Κάτω πλευρά (r = CHUNK_SIZE-1)
        if (c->gridY < CHUNKS_PER_ROW - 1 && map->chunks[i + CHUNKS_PER_COLUMN].walkable) {
            if (c->tiles[CHUNK_SIZE-1][CHUNK_SIZE/2].is_walkable) {
                temp_pts[count++] = c->tiles[CHUNK_SIZE-1][CHUNK_SIZE/2].pos;
            }
        }
        // 3. Αριστερή πλευρά (col = 0)
        if (c->gridX > 0 && map->chunks[i - 1].walkable) {
            if (c->tiles[CHUNK_SIZE/2][0].is_walkable) {
                temp_pts[count++] = c->tiles[CHUNK_SIZE/2][0].pos;
            }
        }
        // 4. Δεξιά πλευρά (col = CHUNK_SIZE-1)
        if (c->gridX < CHUNKS_PER_COLUMN - 1 && map->chunks[i + 1].walkable) {
            if (c->tiles[CHUNK_SIZE/2][CHUNK_SIZE-1].is_walkable) {
                temp_pts[count++] = c->tiles[CHUNK_SIZE/2][CHUNK_SIZE-1].pos;
            }
        }

        // Αν βρήκαμε πύλες, τις αποθηκεύουμε στο chunk
        if (count > 0) {
            c->entrance_count = count;
            c->entrance_points = malloc(sizeof(Vector2) * count);
            memcpy(c->entrance_points, temp_pts, sizeof(Vector2) * count);
        }
    }
}