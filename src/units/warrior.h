#ifndef WARRIOR_H
#define WARRIOR_H

#include "raylib.h"

#include "../engine.h"
#include "./units.h"
#include "../util/util.h"


void DrawWarrior(Unit * self);
UnitData Init_Warrior();
Unit Create_Warrior(UnitData* data,Vector2 startPos,int *id);
#endif