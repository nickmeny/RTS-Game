#include "animation.h"
#include "util.h"


/**
 * @brief This function Take a image path and prefere scale to resize and return a texture2d
 **/
Texture2D TextureLoader(const char *image_path, const float scale_width, const float scale_height)
{
    Image img = LoadImage(image_path);
    ImageResizeNN(&img, img.width * scale_width, img.height * scale_height);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}

void Animator(Animation anim, AnimateState *state, Vector2 pos,bool flip)
{
    // 1. Calculate frame width dynamically based on this specific animation
    float frameWidth = (float)anim.texture.width / anim.frameNum;
    float dt = GetFrameTime();

    Rectangle sourceRec = {
        state->currentFrame * frameWidth,
        0.0f,
        frameWidth,
        (float)anim.texture.height
    };
    if(flip) sourceRec.width*=-1;
    Rectangle dest = { pos.x, pos.y, frameWidth, (float)anim.texture.height };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(anim.texture, sourceRec, dest, origin, 0.0f, WHITE);
    // DrawTextureRec(anim.texture, sourceRec, dest,pos, WHITE);
    // 2. Update the timer and current frame
    state->timer += dt;
    if (state->timer >= UPDATE_TIME)
    {
        state->timer -= UPDATE_TIME;
        state->currentFrame++;
        if (state->currentFrame >= anim.frameNum)
        {
            state->currentFrame = 0;
        }
    }
}

void UnitCollision(Unit* self, Unit* unitList, int count) {
    for (int i = 0; i < count; i++) {
        // Don't check collision with yourself!
        if (self->id == unitList[i].id) continue;

        // Calculate distance between this unit and another
        float dist = Vector2Distance(self->pos, unitList[i].pos);
        float minDist = self->data->HitBoxRadius + unitList[i].data->HitBoxRadius;

        if (dist < minDist) {
            // They are overlapping 
            // Find the direction to push
            Vector2 pushDir = Vector2Normalize(Vector2Subtract(self->pos, unitList[i].pos));
            
            // If they are exactly on top of each other, dist is 0. 
            // Give them a random push so they don't get stuck.
            if (dist == 0) pushDir = (Vector2){ 1, 0 }; 

            //Push them apart slightly (0.5 means both move equally)
            float overlap = minDist - dist;
            self->pos = Vector2Add(self->pos, Vector2Scale(pushDir, overlap * 0.5f));
        }
    }
}