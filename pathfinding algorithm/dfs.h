#ifndef DFS_H
#define DFS_H

#include <stdbool.h>
#include "DFS.c"

bool is_valid_move(int x, int y, unsigned char map[15][19], bool visited[15][19]);
void dfs(unsigned char map[15][19], unsigned char x, unsigned char y, unsigned char dest_x, unsigned char dest_y, int current_length, int *shortest_length, unsigned char path[15][19], bool visited[15][19]);

#endif // DFS_H
