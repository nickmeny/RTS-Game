#ifndef UNITS_H
#define UNITS_H

#include "raylib.h"
#include "../util/animation.h"

//Forward declaration
typedef struct Unit Unit;

//A enum for The state of the Unit
typedef enum {
    STATE_IDLE,
    STATE_RUN,
    STATE_ATTACK,
}UnitState;





typedef struct 
{
    int **costs;
    Point target;
    int unitsAssigned;
}MovementGroup;



//UnitData Struct. This is for the "Image". That struct Initialized one time in start of the game and then it read from the memmory
typedef struct {
    Animation Idle;
    Animation Run;
    Animation Attack;
    //Hitbox-collusion
    Vector2 hitboxOffset;
    float HitBoxRadius;
    int health;
    int speed;
}UnitData;

//Unit struct. THis is for every unit
struct Unit 
{
    bool isSelected; //for the selection
    bool isFacingLeft;
    int id; //a uniq id
    int curHealth; //health of its unit right now
    int ** flowfield;
    UnitData * data;
    Vector2 targetPos;
    Vector2 pos;
    AnimateState State;
    UnitState currentState;
    MovementGroup *group;
    void (*Draw)(Unit *self);
    void (*Update)(Unit *self);
};


#endif