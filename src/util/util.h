#ifndef UTIL_H
#define UTIL_H
#include "../game.h"
#include "raylib.h"
#include "raymath.h"
#include "../units/units.h"




Texture2D TextureLoader(const char * image_path,const float scale_width, const float scale_height);
void UnitCollision(Unit* self, Unit* unitList, int count);

Vector2 Get_Body_Geometry(const Vector2 frame,const SpriteOffsets offs);
Vector2 Get_Frame_W_H(const Animation anim);
Vector2 Find_Center(const Animation anim,const SpriteOffsets offs);
#endif