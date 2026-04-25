#ifndef MAP_H
#define MAP_H

bool ** map_creation(int  size[2]);
void show_map(bool ** map,int size[2]);
void free_table_malloc(void ** table,int h);


#endif