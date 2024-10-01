// dfs.c
#include "dfs.h"
#include <limits.h>

// basically checks if the neighbouring elements havent been visited and available pathways
bool is_valid_move(int x, int y, unsigned char map[15][19], bool visited[15][19]) {
    return x >= 0 && x < 19 && y >= 0 && y < 15 && map[y][x] != 1 && !visited[y][x];
}

// keep in mind this uses a recursive approach of depth first search compared to python
void dfs(unsigned char map[15][19], unsigned char x, unsigned char y, unsigned char dest_x, unsigned char dest_y, int current_length, int *shortest_length, unsigned char path[15][19], bool visited[15][19]) {
    // once we have reached the destination, we check if the current path is shorter than the shortest path
    // and if so we update the shortest path and change the array that shows the path
    if (x == dest_x && y == dest_y) {
        if (current_length < *shortest_length) {
            *shortest_length = current_length;
            for (int i = 0; i < 15; i++) {
                for (int j = 0; j < 19; j++) {
                    if (visited[i][j]) {
                        path[i][j] = 9;
                    } else {
                        path[i][j] = map[i][j];
                    }
                }
            }
        }
        return;
    }

    visited[y][x] = true;

    // making note of surrounding neighbours
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    // checking if the move is valid and if so we recursively call the function
    for (int i = 0; i < 4; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];

        if (is_valid_move(new_x, new_y, map, visited)) {
            dfs(map, new_x, new_y, dest_x, dest_y, current_length + 1, shortest_length, path, visited);
        }
    }

    visited[y][x] = false;
}
