#include <math.h>
#include <stdlib.h>
#include "pathfinding.h"
#include "../engine.h"


float Octile_distance(Chunk a, Chunk b)
{
    int dx = abs(a.gridX - b.gridX);
    int dy = abs(a.gridY - b.gridY);
    if (dx < dy) {
        return dy + 0.414f * dx;
    } else {
        return dx + 0.414f * dy;
    }
}
float GetHierarchicalCost(Chunk* current, Chunk* neighbor) {
    float transition = (current->gridX == neighbor->gridX || current->gridY == neighbor->gridY) ? 1.0f : 1.414f;
    
    // Αν δεν έχουν υπολογιστεί αποστάσεις, βάλε ένα default κόστος
    float internal = (float)CHUNK_SIZE; 

    if (current->inner_distances != NULL && current->entrance_count > 0) {
        float sum = 0;
        int count = 0;
        int n = current->entrance_count;
        
        for (int i = 0; i < n * n; i++) {
            if (current->inner_distances[i] < 10000.0f && current->inner_distances[i] > 0) {
                sum += current->inner_distances[i];
                count++;
            }
        }
        if (count > 0) internal = sum / count;
    }

    return internal + transition;
}

AStarData* AStar_Create(Map* map) {
    AStarData* data = malloc(sizeof(AStarData));
    data->gCost  = malloc(sizeof(float) * CHUNKS_NUMBER);
    data->parent = malloc(sizeof(int)   * CHUNKS_NUMBER);
    data->closed = malloc(sizeof(bool)  * CHUNKS_NUMBER);
    data->path   = malloc(sizeof(int)   * CHUNKS_NUMBER); // χειρότερη περίπτωση
    data->pathLength = 0;
    data->map = map;
    
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        data->gCost[i]  = INFINITY;
        data->parent[i] = -1;
        data->closed[i] = false;
    }
    
    return data;
}

void AStar_Destroy(AStarData* data) {
    free(data->gCost);
    free(data->parent);
    free(data->closed);
    free(data->path);
    free(data);
}

static int CompareFcost(Pointer a, Pointer b) {
    PathNode* nodeA = (PathNode*)a;
    PathNode* nodeB = (PathNode*)b;
    if (nodeA->fCost > nodeB->fCost) return -1;  // μεγαλύτερο fCost = "χειρότερο"
    if (nodeA->fCost < nodeB->fCost) return 1;   // μικρότερο fCost = "καλύτερο"
    return 0;
}

static Chunk* GetChunkById(Map* map, int id) {
    return &map->chunks[id];
}

int* AStar_FindPath(AStarData* data, int startId, int goalId, int* outLength) {
    for (int i = 0; i < CHUNKS_NUMBER; i++) {
        data->gCost[i]  = INFINITY;
        data->parent[i] = -1;
        data->closed[i] = false;
    }
    *outLength = 0;
    
    // Βήμα 1: Έλεγχος εγκυρότητας
    Chunk* startChunk = GetChunkById(data->map, startId);
    Chunk* goalChunk  = GetChunkById(data->map, goalId);
    
    if (!startChunk->walkable || !goalChunk->walkable) {
        return NULL;
    }
    
    // Ταυτότητα; Τότε το μονοπάτι έχει μόνο ένα chunk
    if (startId == goalId) {
        data->path[0] = startId;
        *outLength = 1;
        return data->path;
    }
    printf("ASTAR: start=%d walkable=%d, goal=%d walkable=%d\n",
       startId, startChunk->walkable, goalId, goalChunk->walkable);
    // Βήμα 2: Αρχικοποίηση
    PriorityQueue openSet = pqueue_create(CompareFcost, NULL, NULL);
    
    data->gCost[startId] = 0;
    
    PathNode* startNode = malloc(sizeof(PathNode));
    startNode->chunkId  = startId;
    startNode->parentId = -1;
    startNode->gCost    = 0;
    startNode->fCost    = Octile_distance(*startChunk, *goalChunk);
    pqueue_insert(openSet, startNode);
    
    // Βήμα 3: Το κύριο loop
    while (pqueue_size(openSet) > 0) {
        // 3.1: Πάρε τον καλύτερο κόμβο
        PathNode* current = (PathNode*)pqueue_max(openSet);
        pqueue_remove_max(openSet);
        
        int currentId = current->chunkId;
        
        // 3.2: Βρέθηκε ο προορισμός;
        if (currentId == goalId) {
            free(current);
            
            // Χτίσε το μονοπάτι ακολουθώντας τους parents
            int tempPath[CHUNKS_NUMBER];
            int length = 0;
            int currentPathId = goalId;
            
            while (currentPathId != -1) {
                tempPath[length++] = currentPathId;
                currentPathId = data->parent[currentPathId];
            }
            
            // Αντέστρεψε το μονοπάτι
            for (int i = 0; i < length; i++) {
                data->path[i] = tempPath[length - 1 - i];
            }
            *outLength = length;
            
            pqueue_destroy(openSet);
            return data->path;
        }
        
        // 3.3: Αν είναι ήδη κλειστός, skip
        if (data->closed[currentId]) {
            free(current);
            continue;
        }
        
        // 3.4: Κλείσ' τον
        data->closed[currentId] = true;
        
        // 3.5: Εξέτασε όλους τους γείτονες
        Chunk* currentChunk = GetChunkById(data->map, currentId);
        
        for (int i = 0; i < currentChunk->neighborCount; i++) {
            Chunk* neighbor = currentChunk->neighbors[i];
            int neighborId = neighbor->id;
            
            // Αγνόησε αν δεν είναι walkable ή είναι κλειστός
            if (!neighbor->walkable || data->closed[neighborId]) {
                continue;
            }
            
            // Υπολόγισε το νέο gCost
            float moveCost = GetHierarchicalCost(currentChunk, neighbor);
            float tentativeGCost = current->gCost + moveCost;
            
            // Αν βρήκαμε καλύτερο μονοπάτι
            if (tentativeGCost < data->gCost[neighborId]) {
                data->gCost[neighborId]  = tentativeGCost;
                data->parent[neighborId] = currentId;
                
                float hCost = Octile_distance(*neighbor, *goalChunk);
                
                PathNode* neighborNode = malloc(sizeof(PathNode));
                neighborNode->chunkId  = neighborId;
                neighborNode->parentId = currentId;
                neighborNode->gCost    = tentativeGCost;
                neighborNode->fCost    = tentativeGCost + hCost;
                
                pqueue_insert(openSet, neighborNode);
            }
        }
        
        free(current);
    }
    
    // Βήμα 4: Δεν βρέθηκε μονοπάτι
    pqueue_destroy(openSet);
    *outLength = 0;
    return NULL;
}

Vector2 GetChunkConnectionPoint(Chunk* a, Chunk* b) {
    // Βρίσκουμε το κοινό σύνορο μεταξύ των δύο chunks
    float chunkPx = CHUNK_SIZE * TILE_SIZE;
    
    // Μια απλή προσέγγιση: το σημείο ανάμεσα στα κέντρα τους
    Vector2 posA = { a->gridX * chunkPx + chunkPx/2.0f, a->gridY * chunkPx + chunkPx/2.0f };
    Vector2 posB = { b->gridX * chunkPx + chunkPx/2.0f, b->gridY * chunkPx + chunkPx/2.0f };
    
    return (Vector2){ (posA.x + posB.x) / 2.0f, (posA.y + posB.y) / 2.0f };
}

Vector2 GetNextWaypoint(Map* map, int currentChunkId, int nextChunkId) {
    Chunk* a = &map->chunks[currentChunkId];
    Chunk* b = &map->chunks[nextChunkId];

    // Ψάχνουμε μια πύλη του Chunk A που να ακουμπάει το Chunk B
    for (int i = 0; i < a->entrance_count; i++) {
        Vector2 p = a->entrance_points[i];
        
        // Αν η πύλη βρίσκεται στο σύνορο με το επόμενο chunk
        // Ελέγχουμε αν το σημείο p περιέχεται στα όρια του επόμενου chunk (με μια μικρή ανοχή)
        if (CheckCollisionPointRec(p, b->bounds)) {
            return p;
        }
    }

    // Fallback: Αν δεν βρει πύλη (σπάνιο), πήγαινε στο κέντρο του επόμενου chunk
    return (Vector2){ 
        b->bounds.x + b->bounds.width/2, 
        b->bounds.y + b->bounds.height/2 
    };
}