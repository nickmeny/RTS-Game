#ifndef ANIMATION_H
#define ANIMATION_H

#include "raylib.h"
#include "../basic_headers/basics.h"
//Animation struct is used to store the spreetshit for every animation and the frames 
typedef struct 
{
    Texture2D texture;
    int frameNum;
}Animation;


//AnimationState is the struct to store the speed and the current Frame of a animation
typedef struct {
    int currentFrame;
    float timer;
}AnimateState;

void Animator( Animation anim,AnimateState *state, Vector2 pos,bool flip);

#endif