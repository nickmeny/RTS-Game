#ifndef ENGINE_H
#define ENGINE_H

#define S_WIDTH 800
#define S_HEIGHT 450
#define UPDATE_TIME 0.2f
#define MAP_HEIGHT 2000
#define MAP_WIDTH 2000
#define TILE_SIZE 64
#define CHUNK_SIZE 16
#define CHUNKS_TILES_COUNT (CHUNK_SIZE*CHUNK_SIZE)
#define CHUNK_SIZE_PX (CHUNK_SIZE * TILE_SIZE) 
#define CHUNKS_PER_ROW (MAP_HEIGHT/CHUNK_SIZE)
#define CHUNKS_PER_COLUMN (MAP_WIDTH/CHUNK_SIZE)
#define CHUNKS_NUMBER (CHUNKS_PER_COLUMN*CHUNKS_PER_ROW)

//===========================================MACROS============================================
// Source - https://stackoverflow.com/a/3437484
//this macro is to find the max of 2 numbers
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

//=============================================================================================

#endif