#include <math.h>
#include "map.h"
#include "stdlib.h"
#include "../stractures/priority_queue.h" 
#include "../engine.h"


// Μικρό struct για τους τοπικούς κόμβους (μόνο για εσωτερική χρήση)
typedef struct {
    int r, c;
    float gCost;
    float fCost;
} LocalNode;

// Απόσταση μεταξύ tiles (r1,c1) και (r2,c2)
static float tile_dist(int r1, int c1, int r2, int c2) {
    int dr = abs(r1 - r2);
    int dc = abs(c1 - c2);
    return (dr > dc) ? (dc * 1.414f + (dr - dc)) : (dr * 1.414f + (dc - dr));
}

static int CompareFcost(Pointer a, Pointer b) {
    LocalNode* nodeA = (LocalNode*)a;
    LocalNode* nodeB = (LocalNode*)b;
    if (nodeA->fCost > nodeB->fCost) return -1;  // μεγαλύτερο fCost = "χειρότερο"
    if (nodeA->fCost < nodeB->fCost) return 1;   // μικρότερο fCost = "καλύτερο"
    return 0;
}

float local_tile_astar(Chunk *chunk, Vector2 startWorldPos, Vector2 goalWorldPos) {
    // 1. Μετατροπή World Pos σε Local Tile Indices (0-31)
    int startC = ((int)startWorldPos.x % (CHUNK_SIZE * TILE_SIZE)) / TILE_SIZE;
    int startR = ((int)startWorldPos.y % (CHUNK_SIZE * TILE_SIZE)) / TILE_SIZE;
    int goalC = ((int)goalWorldPos.x % (CHUNK_SIZE * TILE_SIZE)) / TILE_SIZE;
    int goalR = ((int)goalWorldPos.y % (CHUNK_SIZE * TILE_SIZE)) / TILE_SIZE;

    // 2. Data structures για 32x32 (στατικοί πίνακες για ταχύτητα, είναι μόνο 1KB)
    float gCost[CHUNK_SIZE][CHUNK_SIZE];
    bool closed[CHUNK_SIZE][CHUNK_SIZE];
    for (int r = 0; r < CHUNK_SIZE; r++) {
        for (int c = 0; c < CHUNK_SIZE; c++) {
            gCost[r][c] = INFINITY;
            closed[r][c] = false;
        }
    }

    PriorityQueue openSet = pqueue_create(CompareFcost, NULL, NULL); // Χρησιμοποίησε την CompareFcost που έχεις

    gCost[startR][startC] = 0;
    LocalNode* startNode = malloc(sizeof(LocalNode));
    startNode->r = startR; startNode->c = startC;
    startNode->gCost = 0;
    startNode->fCost = tile_dist(startR, startC, goalR, goalC);
    pqueue_insert(openSet, startNode);

    while (pqueue_size(openSet) > 0) {
        LocalNode* current = (LocalNode*)pqueue_max(openSet);
        pqueue_remove_max(openSet);

        if (current->r == goalR && current->c == goalC) {
            float finalDist = current->gCost;
            free(current);
            pqueue_destroy(openSet); // Καθάρισμα
            return finalDist;
        }

        if (closed[current->r][current->c]) { free(current); continue; }
        closed[current->r][current->c] = true;

        // Έλεγχος 8 γειτόνων
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;

                int nr = current->r + dr;
                int nc = current->c + dc;

                // Όρια Chunk και Walkable check
                if (nr >= 0 && nr < CHUNK_SIZE && nc >= 0 && nc < CHUNK_SIZE) {
                    if (!chunk->tiles[nr][nc].is_walkable || closed[nr][nc]) continue;
                    if (abs(dr) + abs(dc) == 2) { // Αν η κίνηση είναι διαγώνια
                        // Έλεγχος αν "γδέρνει" τοίχο (τα δύο tiles που σχηματίζουν τη γωνία)
                        if (!chunk->tiles[current->r + dr][current->c].is_walkable || 
                            !chunk->tiles[current->r][current->c + dc].is_walkable) {
                            continue; // Απόρριψε τη διαγώνια κίνηση
                        }
                    }
                    float weight = (dr == 0 || dc == 0) ? 1.0f : 1.414f;
                    float newGCost = current->gCost + weight;

                    if (newGCost < gCost[nr][nc]) {
                        gCost[nr][nc] = newGCost;
                        LocalNode* neighbor = malloc(sizeof(LocalNode));
                        neighbor->r = nr; neighbor->c = nc;
                        neighbor->gCost = newGCost;
                        neighbor->fCost = newGCost + tile_dist(nr, nc, goalR, goalC);
                        pqueue_insert(openSet, neighbor);
                    }
                }
            }
        }
        free(current);
    }

    pqueue_destroy(openSet);
    return INFINITY; // Δεν βρέθηκε μονοπάτι μέσα στο chunk
}