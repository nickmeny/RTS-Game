#ifndef MAP_H
#define MAP_H

#include <raylib.h>


typedef struct {
    bool is_walkable;
    Vector2 pos;
}Tile;

typedef struct  chunk {
    int id;
    bool walkable;
    Tile ** tiles;
    Tile * entrance_tiles; 
    int gridX,gridY;
    Rectangle bounds;
    int neighborCount;
    struct chunk ** neighbors;
}Chunk;

typedef struct
{
    Chunk * chunks;
} Map;

Map create_map();
void DrawChunksFromMemory(Map* graph, Camera2D* cam);
void free_map(Map map);
#endif