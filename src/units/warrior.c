#include <raylib.h>
#include "warrior.h"
#include <stdlib.h>
#include <math.h>      // <-- για το sqrtf



//That func is the CallBack for the Struct Unit
void DrawWarrior(Unit * self)
{
    // To default animation
    Animation *currentAnim = &self->data->Idle;
    int anim_index = 0;//αυτο το index ειναι για το body struct 
    
    switch (self->currentState)
    {
    case STATE_RUN:
        currentAnim = &self->data->Run;
        anim_index = 1;
        break;
    case STATE_IDLE:
        currentAnim= &self->data->Idle;
        anim_index =0;
        break;
    case STATE_ATTACK:
        currentAnim = &self->data->Attack;
        anim_index = 2;
        break;
    default:
        break;
    }
    
    //Εδω απλα μια τοπικη μεταβλητη για να ειναι πιο Clean
    Body body = self->data->body;
    
    //Αν ειναι επιλεγμενο
    if (self->isSelected) {
        //Υπολογιζω που ειναι τα ποδια. Ειναι το πανω offset + το σωμα 
        float feetY = WARRIOR_IDLE_MAX_Y_OFFSET +  body.bodyGeometry[anim_index].y;
        Vector2 feet = { 
            self->pos.x + body.bodyCenter[anim_index].x, // το παγκοσμιο x + το center
            self->pos.y + feetY 
        };
        
        // Draw the ring (using your HitBoxRadius for the size!)
        DrawEllipseLines(feet.x, feet.y, self->data->HitBoxRadius, 5, GREEN); 
    }
    Rectangle collision = {
        
        self->pos.x + WARRIOR_IDLE_MIN_X_OFFSET,      // x στον κόσμο
        self->pos.y + WARRIOR_IDLE_MIN_Y_OFFSET,      // y στον κόσμο
        body.bodyGeometry[anim_index].x,               // πλάτος σώματος
        body.bodyGeometry[anim_index].y   
        
    };
    DrawRectangleLinesEx(collision,3,RED);
    Animator(*currentAnim,&self->State,self->pos,self->isFacingLeft);

}

void UpdateWarrior(Unit * self)
{
    float dx = self->targetPos.x - self->pos.x;
    float dy = self->targetPos.y - self->pos.y;
    float dist = sqrtf(dx*dx + dy*dy);

    if (dist > 5.0f) 
    {
        self->pos.x += (dx / dist) * self->data->speed * GetFrameTime();
        self->pos.y += (dy / dist) * self->data->speed * GetFrameTime();
        
        self->currentState = STATE_RUN;
        if (dx < 0) self->isFacingLeft = true;
        else if (dx > 0) self->isFacingLeft = false;
    }
    else
    {
        // 2. ΕΔΩ ΕΙΝΑΙ Η ΚΡΙΣΙΜΗ ΑΛΛΑΓΗ
        if (self->path != NULL && self->currentPathIndex < self->pathLength)
        {
            int currentChunkId = self->path[self->currentPathIndex];
            
            // Έλεγχος αν το ID είναι έγκυρο πριν το χρησιμοποιήσουμε
            if (currentChunkId < 0 || currentChunkId >= CHUNKS_NUMBER) {
                free(self->path);
                self->path = NULL;
                self->pathLength = 0;
                self->currentState = STATE_IDLE;
                return;
            }

            if (self->currentPathIndex < self->pathLength - 1)
            {
                int nextChunkId = self->path[self->currentPathIndex + 1];
                
                // Διπλοτσεκάρουμε και το επόμενο ID
                if (nextChunkId >= 0 && nextChunkId < CHUNKS_NUMBER) {
                    self->targetPos = GetNextWaypoint(self->map, currentChunkId, nextChunkId);
                    self->currentPathIndex++;
                }
            }
            else 
            {
                // Τελευταίο chunk
                self->targetPos = self->finalTarget;
                free(self->path);
                self->path = NULL;
                self->pathLength = 0;
            }
        }
        else
        {
            self->currentState = STATE_IDLE;
        }
    }
}
//This Func returns a UnitData initializetion.
UnitData Init_Warrior()
{
    UnitData data = {0};
    
    //Εδω κανω τα malloc για το body
    data.body.bodyCenter = malloc(sizeof(Vector2)*3);
    data.body.bodyGeometry = malloc(sizeof(Vector2)*3);
    data.body.frameGeometry = malloc(sizeof(Vector2)*3);
    
    //Οτι πρεπει για το Idle
    data.Idle.texture = TextureLoader("assets/Units/Black_Units/Warrior/Warrior_Idle.png", 1.0f, 1.0f);
    data.Idle.frameNum = 8;  
    data.body.frameGeometry[0] = Get_Frame_W_H(data.Idle);
    data.body.bodyGeometry[0] = Get_Body_Geometry(data.body.frameGeometry[0],WARRIOR_IDLE_OFFSETS);
    data.body.bodyCenter[0] = Find_Center(data.Idle,WARRIOR_IDLE_OFFSETS);

    //RUN
    data.Run.texture = TextureLoader("assets/Units/Black_Units/Warrior/Warrior_Run.png", 1.0f, 1.0f);
    data.Run.frameNum = 6;
    data.body.frameGeometry[1] = Get_Frame_W_H(data.Run);
    data.body.bodyGeometry[1] = Get_Body_Geometry(data.body.frameGeometry[1],WARRIOR_IDLE_OFFSETS);
    data.body.bodyCenter[1] = Find_Center(data.Run,WARRIOR_IDLE_OFFSETS);

    //Others
    data.HitBoxRadius = 30.0f; 
    data.health = 100;
    data.speed = 300;

    
    return data;
}

//That create the Unit in memory
Unit Create_Warrior(UnitData* data,Vector2 startPos,int *id,Map* map)
{
    Unit w = {0}; // Initialize all to zero
    w.id = *id;
    w.pos = startPos;
    w.curHealth = data->health;
    w.data = data;
    w.currentState = STATE_IDLE;
    w.isFacingLeft = false;
    w.State = (AnimateState){0};
    w.path = NULL;
    w.pathLength = 0;
    w.currentPathIndex = 0;
    w.map = map;
    w.Draw = DrawWarrior; 
    w.Update = UpdateWarrior;
    w.targetPos = startPos;
    return w;
}



