#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool ** map_creation(int  size[2])
{
    int h = size[0];
    int w = size[1];
    bool ** map = malloc(h*sizeof(bool*));
    if (map==NULL)
    {
        return NULL;
    }
    for (int i=0;i<h;i++){
        map[i] = malloc(w*sizeof(bool));
        if(map[i]==NULL){
            for (int j=0;j<i;j++)
            {
                free(map[j]);
            }
            free(map);
        }
        memset(map[i],false,w);
    }
    return map;
}

void show_map(bool ** map, int size[2])
{
    int rows = size[0];
    int cols = size[1];
    for(int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Calculate rectangle area
            Rectangle tileRect = { (float)j * 32, (float)i * 32, 31, 31 };

            if (map[i][j] == true) {
                // Draw blocked tiles as solid colors
                DrawRectangleRec(tileRect, DARKGRAY);
            } else {
                // Draw walkable tiles as just a faint outline
                DrawRectangleLinesEx(tileRect, 1.0f, Fade(LIGHTGRAY, 0.5f));
            }
        }
    }
}


void free_table_malloc(void ** table,int h)
{
    for (int i=0;i<h;i++)
    {
        free(table[i]);
    }
    free(table);
    
}