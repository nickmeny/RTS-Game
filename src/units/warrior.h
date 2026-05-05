#ifndef WARRIOR_H
#define WARRIOR_H

#include "raylib.h"

#include "../engine.h"
#include "./units.h"
#include "../util/util.h"
#include "../basic_headers/basics.h"
#include "../util/offsets.h"

void DrawWarrior(Unit * self);
UnitData Init_Warrior();
Unit Create_Warrior(UnitData* data,Vector2 startPos,int *id,Map * map);

static const SpriteOffsets WARRIOR_IDLE_OFFSETS = {
    WARRIOR_IDLE_MIN_X_OFFSET,
    WARRIOR_IDLE_MAX_X_OFFSET,
    WARRIOR_IDLE_MIN_Y_OFFSET,
    WARRIOR_IDLE_MAX_Y_OFFSET
};
#endif