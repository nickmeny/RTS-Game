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


/// @brief Func για να βρισκω το μεγεθος που εχει ενα frame.
/// @param frame Vector2, εχει το Height και το Width
/// @param offs SpriteOffsets, τα offsets του Sprite 
/// @return Vector2
Vector2 Get_Body_Geometry(const Vector2 frame,const SpriteOffsets offs)
{
    float bodyWidth  = frame.x- offs.minX - offs.maxX;
    float bodyHeight = frame.y - offs.minY-offs.maxY;
    return (Vector2){bodyWidth,bodyHeight};
}


/// @brief Func για να παρινω το Width και το Height απο ενα texture
/// @param anim 
/// @return Vector2
Vector2 Get_Frame_W_H(const Animation anim)
{
    float frameWidth = (float)anim.texture.width / anim.frameNum;
    float frameHeight = (float)anim.texture.height;
    return (Vector2){frameWidth,frameHeight};
}

/// @brief Συναρτηση για να βρισκω το κεντρο του sprite(Μονο της μορφης που υπαρχει, οχι γενικα του texture)
/// @param anim Animation struct
/// @param offs SpriteOffsets, τα offsets του Sprite 
/// @return Vector2
Vector2 Find_Center(const Animation anim,const SpriteOffsets offs){
    Vector2 frame = Get_Frame_W_H(anim);
    Vector2 body = Get_Body_Geometry(frame,offs);
    //το -3 το εχω βαλει επειδη ναι μεν οι υπολογισμοι ειναι σωστοι
    // αλλα στο ματι φαινεται καλυτερα αν παει 3px ποιο εκει
    float centerX = offs.minX + (body.x / 2.0f) - 3;
    float centerY = offs.minY + (body.y / 2.0f);

    return (Vector2){centerX,centerY};  
}
