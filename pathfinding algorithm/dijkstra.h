#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdbool.h>
#include "Dijkstra.c"

void dijkstra(unsigned char map[15][19], unsigned char start_x, unsigned char start_y, unsigned char dest_x, unsigned char dest_y, unsigned char path[15][19], int *shortest_length);

#endif 
