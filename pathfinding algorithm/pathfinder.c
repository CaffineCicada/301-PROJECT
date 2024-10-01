// main.c
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "dfs.h"
#include "Astar.h"
#include "bfs.h"
#include "dijkstra.h"

void add_positions_to_map(unsigned char map[15][19], unsigned char start_pos[2], unsigned char food_list[5][2]) {
    map[start_pos[1]][start_pos[0]] = 2;
    for (int i = 0; i < sizeof(food_list)/sizeof(food_list[0]); i++) {
        map[food_list[i][1]][food_list[i][0]] = 3;
    }
}

void print_map(unsigned char map[15][19]) {
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            printf("%2d", map[y][x]);
        }
        printf("\n");
    }
}

int main() {
    unsigned char map[15][19] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1},
        {1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1},
        {1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1},
        {1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1},
        {1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    unsigned char start_pos[2] = {1, 1};

    unsigned char food_list[5][2] = {
        {1, 9},
        {5, 5},
        {7, 1},
        {13, 5},
        {9, 9}
    };

    unsigned char original_map[15][19];
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 19; j++) {
            for (int k = 0; k < sizeof(food_list)/sizeof(food_list[0]); k++) {
            map[food_list[k][1]][food_list[k][0]] = 3;
            }
            original_map[i][j] = map[i][j];
        }
    }

    add_positions_to_map(map, start_pos, food_list);

    unsigned char path[15][19] = {0};
    bool visited[15][19] = {false};
    int shortest_length = INT_MAX;

    for (int i = 0; i < sizeof(food_list)/sizeof(food_list[0]); i++) {
        for (int y = 0; y < 15; y++) {
            for (int x = 0; x < 19; x++) {
                visited[y][x] = false;
            }
        }

        for (int y = 0; y < 15; y++) {
            for (int x = 0; x < 19; x++) {
                if (path[y][x] == 9) {
                    path[y][x] = 0;
                }
            }
        }

        for (int y = 0; y < 15; y++) {
            for (int x = 0; x < 19; x++) {
                map[y][x] = original_map[y][x];
            }
        }

        map[start_pos[1]][start_pos[0]] = 2;
        
        bfs(map, start_pos[0], start_pos[1], food_list[i][0], food_list[i][1], path, &shortest_length);

        printf("Path to node %d (%d, %d):\n", i + 1, food_list[i][0], food_list[i][1]);
        print_map(path);
        printf("Length of shortest path: %d\n\n", shortest_length);

        map[food_list[i][1]][food_list[i][0]] = 4;

        start_pos[0] = food_list[i][0];
        start_pos[1] = food_list[i][1];

        shortest_length = INT_MAX;
    }

    return 0;
}
