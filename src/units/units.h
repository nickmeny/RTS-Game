#ifndef UNITS_H
#define UNITS_H

#include "raylib.h"
#include "../util/animation.h"
#include "../basic_headers/basics.h"
#include "../map/map.h"
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

//This struct is to save the basic geometry of a unit in all states
typedef struct {
    Vector2* bodyGeometry; // A table to store the body height and width for all animations the Unit has
    Vector2* bodyCenter; // A table to store the center of a Unit for all of its animation
    Vector2* frameGeometry; // A table to store the frame Height and Width for its Animation
}Body;

//UnitData Struct. This is for the "Image". That struct Initialized one time in start of the game and then it read from the memmory
typedef struct {
    //Animations
    Animation Idle;
    Animation Run;
    Animation Attack;
    //Hitbox-collusion
    Vector2 hitboxOffset;
    float HitBoxRadius;
    int health;
    int speed;

    //Sprite Attributes
    Body body;
    
}UnitData;

//Unit struct. THis is for every unit
struct Unit 
{
    bool isSelected; //for the selection
    bool isFacingLeft;
    
    int id; //a uniq id
    int curHealth; //health of its unit right now
    int*  path;            // πίνακας από chunkIds
    int   pathLength;      
    int   currentPathIndex;
    
    UnitData * data;
    Vector2 targetPos;
    Vector2 pos;
    Vector2 finalTarget; 
    AnimateState State;
    UnitState currentState;
    MovementGroup *group;
    Map * map;
    
    void (*Update)(Unit *self);
    void (*Draw)(Unit *self);
};


#endif