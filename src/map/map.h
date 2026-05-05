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
    Vector2  *entrance_points; 
    int entrance_count;
    float *inner_distances;
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

float local_tile_astar(Chunk *chunk, Vector2 startWorldPos, Vector2 goalWorldPos);
void build_entrances(Map *map);
void compute_intra_edges(Chunk *chunk);
#endif