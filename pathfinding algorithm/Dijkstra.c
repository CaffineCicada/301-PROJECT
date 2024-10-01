#include "dijkstra.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define ROWS 15  
#define COLS 19  

// Pointer to represent x and y position in maze
typedef struct {
    int x, y;
} Point;

// Struct to represent each node in the maze
typedef struct {
    int distance;    
    Point prev;     
    bool visited;    
} Node;

// initiallise all nodes
void initialize_nodes(Node nodes[ROWS][COLS], unsigned char map[ROWS][COLS]) {
    for (int y = 0; y < ROWS; y++) {  
        for (int x = 0; x < COLS; x++) {
            nodes[y][x].distance = INT_MAX;
            nodes[y][x].prev.x = -1; 
            nodes[y][x].prev.y = -1; 
            nodes[y][x].visited = false; 
        }
    }
}

// just checking if the next node is a valid place to move
bool is_valid(int x, int y, unsigned char map[ROWS][COLS]) {
    return (x >= 0 && x < COLS && y >= 0 && y < ROWS && map[y][x] != 1);  
    }


void dijkstra(unsigned char map[ROWS][COLS], unsigned char start_x, unsigned char start_y, unsigned char dest_x, unsigned char dest_y, unsigned char path[ROWS][COLS], int *shortest_length) {
    Node nodes[ROWS][COLS];  
    initialize_nodes(nodes, map);  

    nodes[start_y][start_x].distance = 0;  

    // gives our possible directions
    Point directions[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    Point queue[ROWS * COLS];  
    int queue_start = 0, queue_end = 0;  
    
    // adds the start node to the queue
    queue[queue_end++] = (Point){start_x, start_y};  

    // loops until the queue is empty
    while (queue_start < queue_end) {
        Point current = queue[queue_start++];  
        int cx = current.x;
        int cy = current.y;  

        // if the start node and end node match, we have found the shortest path
        if (cx == dest_x && cy == dest_y) {
            break;
        }

        // skip node if its been visited
        if (nodes[cy][cx].visited) {
            continue;
        }

        // makes the current node as visited
        nodes[cy][cx].visited = true;  

        // going through all possible directions
        for (int i = 0; i < 4; i++) {
            int nx = cx + directions[i].x;  
            int ny = cy + directions[i].y;  

            // Check if the new position is valid and hasn't been visited
            if (is_valid(nx, ny, map) && !nodes[ny][nx].visited) {
                int new_dist = nodes[cy][cx].distance + 1;  

                // check if the new distance is shorter and update the distance and add to queue
                if (new_dist < nodes[ny][nx].distance) {
                    nodes[ny][nx].distance = new_dist;  
                    nodes[ny][nx].prev.x = cx;  
                    nodes[ny][nx].prev.y = cy;  
                    queue[queue_end++] = (Point){nx, ny};  
                }
            }
        }
    }

    int path_length = nodes[dest_y][dest_x].distance;  
    
    // if no possible path was found
    if (path_length == INT_MAX) {
        printf("No path found.\n");  
        path_length = -1;  
    } else {
        
        // copy the maze into the path  
        for (int y = 0; y < ROWS; y++) {
            for (int x = 0; x < COLS; x++) {
                path[y][x] = map[y][x];
            }
        }

        // update the path showing the shortest possible distance to the end node
        Point current = (Point){dest_x, dest_y};
        while (current.x != start_x || current.y != start_y) {
            path[current.y][current.x] = 9;  
            current = nodes[current.y][current.x].prev; 
        }
        path[start_y][start_x] = 2; 
    }

    *shortest_length = path_length;  
}
