#ifndef GAME_H
#define GAME_H


#include "engine.h"
#include "units/warrior.h"
#include <raylib.h>
#include <raymath.h>
#include "stractures/simple_link_list.h"
#include "./util/pathfinding.h"

typedef struct {
    Unit *unitList;
    int *unitCount;
    int maxUnits;
    UnitData *warriorData;
    LinkList groupList;

    // State για το UI
    bool *isDragging;
    Vector2 *boxStartPoint;
    Camera2D *cam;
    Map* map;
    AStarData* astar;
}GameContext;

#endif
