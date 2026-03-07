#ifndef PATHFINFING_H
#define PATHFINDING_H

#include "../stractures/queue.h" 
#include <raylib.h>

void generate_dijkstar_map(Queue *q,int **costs,bool ** map,Point target,int h,int w);
Vector2 get_flow_direction(int ** costs,int x, int y,int h,int w);


#endif