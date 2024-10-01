#ifndef BFS_H
#define BFS_H

#include <stdbool.h>
#include "BFS.c"

void bfs(unsigned char map[15][19], unsigned char start_x, unsigned char start_y, unsigned char dest_x, unsigned char dest_y, unsigned char path[15][19],int *shortest_length);

#endif 