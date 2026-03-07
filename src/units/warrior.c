#include <raylib.h>
#include "warrior.h"


//That func is the CallBack for the Struct Unit
void DrawWarrior(Unit * self)
{
    Animation *currentAnim = &self->data->Idle;
    
    switch (self->currentState)
    {
    case STATE_RUN:
        currentAnim = &self->data->Run;
        break;
    case STATE_IDLE:
        currentAnim= &self->data->Idle;
        break;
    case STATE_ATTACK:
        currentAnim = &self->data->Attack;
        break;
    default:
        break;
    }
    
    if (self->isSelected) {
        // Calculate the center-bottom of the current frame
        float frameWidth = (float)currentAnim->texture.width / currentAnim->frameNum;
        float frameHeight = (float)currentAnim->texture.height;
        
        Vector2 feet = { 
            self->pos.x + (frameWidth / 2.0f), 
            self->pos.y + (frameHeight * 0.9f) // 90% down the sprite
        };
        
        // Draw the ring (using your HitBoxRadius for the size!)
        DrawEllipseLines(feet.x, feet.y, self->data->HitBoxRadius, 5, GREEN); 
    }
    Animator(*currentAnim,&self->State,self->pos,self->isFacingLeft);

}


void UpdateWarrior(Unit * self){
    if(self->currentState == STATE_RUN){
        int tx = (int)(self->pos.x/32);
        int ty = (int)(self->pos.y/32);
        if(tx>= 0 && tx<MAP_HEIGHT && ty >=0 && ty < MAP_HEIGHT)
        {
            if(self->flowfield[ty][tx] == 0)
            {
                self->currentState = STATE_IDLE;
                return;
            }
            Vector2 dir = get_flow_direction(self->flowfield,tx,ty,MAP_HEIGHT,MAP_WIDTH);
            self->pos.x += dir.x * self->data->speed * GetFrameTime();
            self->pos.y += dir.y * self->data->speed * GetFrameTime();
            if (dir.x < 0) self->isFacingLeft = true;
            else if (dir.x > 0) self->isFacingLeft = false;
        }
    }
}
//This Func returns a UnitData initializetion.
UnitData Init_Warrior()
{
    UnitData data = {0};
    
    data.Idle.texture = TextureLoader("assets/Units/Black_Units/Warrior/Warrior_Idle.png", 0.5f, 0.5f);
    data.Idle.frameNum = 8;  
 
    data.Run.texture = TextureLoader("assets/Units/Black_Units/Warrior/Warrior_Run.png", 0.5f, 0.5f);
    data.Run.frameNum = 6;

    // Set a consistent radius (e.g., 15 pixels after 0.5 scaling)
    data.HitBoxRadius = 15.0f; 
    data.health = 100;
    data.speed = 300;

    return data;
}

//That create the Unit in memory
Unit Create_Warrior(UnitData* data,Vector2 startPos,int *id)
{
    Unit w = {0}; // Initialize all to zero
    w.id = *id;
    w.pos = startPos;
    w.curHealth = data->health;
    w.data = data;
    w.currentState = STATE_IDLE;
    w.isFacingLeft = false;
    w.State = (AnimateState){0};
    w.Draw = DrawWarrior; 
    w.Update = UpdateWarrior;
    return w;
}
