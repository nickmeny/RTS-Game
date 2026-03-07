#ifndef GAME_H
#define GAME_H


#include "engine.h"
#include "units/warrior.h"
#include "util/pathfinding.h"
#include "map.h"
#include <raylib.h>
#include <raymath.h>
#include "controls.h"
#include "stractures/simple_link_list.h"

typedef struct {
    Unit *unitList;
    int *unitCount;
    int maxUnits;
    UnitData *warriorData;
    LinkList groupList;

    // Pathfinding δεδομένα
    Queue *flowQueue;
    int **costs;
    bool **map;
    int mapW;
    int mapH;

    // State για το UI
    bool *isDragging;
    Vector2 *boxStartPoint;
}GameContext;

#endif
