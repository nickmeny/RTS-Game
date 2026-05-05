#include <stdio.h>
#include "../map/map.h"

#include "../stractures/priority_queue.h"

// Το struct για κάθε κόμβο στην ανοιχτή λίστα
typedef struct {
    int   chunkId;
    int   parentId;
    float gCost;
    float fCost;
} PathNode;

// Το struct για τα δεδομένα του Α*
typedef struct {
    float* gCost;
    int*   parent;
    bool*  closed;
    int*   path;
    int    pathLength;
    Map*   map;
} AStarData;

// Συναρτήσεις
AStarData* AStar_Create(Map* map);
void AStar_Destroy(AStarData* data);
int* AStar_FindPath(AStarData* data, int startId, int goalId, int* outLength);

// Βοηθητικές
float Octile_distance(Chunk a, Chunk b);
Vector2 GetNextWaypoint(Map* map, int currentChunkId, int nextChunkId);
// int CompareFcost(Pointer a, Pointer b);