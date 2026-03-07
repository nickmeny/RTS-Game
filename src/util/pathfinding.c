#include "../stractures/queue.h" 
#include <raylib.h>
#include <raymath.h>

void generate_dijkstar_map(Queue *q,int **costs,bool ** map,Point target,int h,int w)
{
    q->front = -1;
    q->rear = -1;
    for (int i =0 ; i<h;i++)
    {
        for (int j=0;j<w;j++)
        {
            costs[i][j] = 99999;
        }
    }
    costs[target.y][target.x] = 0;
    insert_queue(q,target);
    while (!is_empty(q))
    {
        Point curr = delete_queue(q);
        int dx[] = {0,0,1,-1};
        int dy[] = {1,-1,0,0};
        for (int i=0;i<4;i++)
        {
            Point next = (Point){curr.x + dx[i],curr.y+ dy[i]};
            if(next.x>=0 && next.x<w && next.y>= 0 && next.y<h)
            {
                if(!map[next.y][next.x] && costs[next.y][next.x]>costs[curr.y][curr.x]+1)
                {
                    costs[next.y][next.x] = costs[curr.y][curr.x]+1;
                    insert_queue(q,next);
                }
            }
        }
    }
}


Vector2 get_flow_direction(int ** costs,int x, int y,int h,int w)
{
    float dx = 0, dy =0;
    if (x <= 0 || x >= w - 1 || y <= 0 || y >= h - 1)
    {
        return (Vector2){0,0};
    }
    dx = (costs[y-1][x-1] + costs[y][x-1] + costs[y+1][x-1]) - 
         (costs[y-1][x+1] + costs[y][x+1] + costs[y+1][x+1]);

    dy = (costs[y-1][x-1] + costs[y-1][x] + costs[y-1][x+1]) - 
         (costs[y+1][x-1] + costs[y+1][x] + costs[y+1][x+1]);
    
    Vector2 dir =(Vector2){dx,dy};

    return Vector2Normalize(dir);
}