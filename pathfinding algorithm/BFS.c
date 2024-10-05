#include "bfs.h"
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_QUEUE_SIZE 1000


// since c has no libraries for queue, we have to implement our own queue
typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[MAX_QUEUE_SIZE];
    int front, rear;
} Queue;

static void enqueue(Queue* queue, Point p) {
    queue->points[queue->rear++] = p;
}

static Point dequeue(Queue* queue) {
    return queue->points[queue->front++];
}

static bool is_empty(Queue* queue) {
    return queue->front == queue->rear;
}
// end of queue implementation


static bool is_valid_move(int x, int y, unsigned char map[15][19], bool visited[15][19]) {
    return x >= 0 && x < 19 && y >= 0 && y < 15 && map[y][x] != 1 && !visited[y][x];
}

// this pretty much updates the map with the shortest path to the end node
static void print_path(Point prev[15][19], Point start, Point end, unsigned char path[15][19]) {
    Point current = end;
    while (current.x != start.x || current.y != start.y) {
        path[current.y][current.x] = 9;
        current = prev[current.y][current.x];
    }
    path[start.y][start.x] = 2;
}

void bfs(unsigned char map[15][19], unsigned char start_x, unsigned char start_y, unsigned char dest_x, unsigned char dest_y, unsigned char path[15][19], int *shortest_length, char turns[15][19]) {
    Queue queue = {{0}, 0, 0};
    bool visited[15][19];
    Point prev[15][19];
    int dist[15][19];
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    // Initialize the visited, prev, dist, and turns arrays
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            visited[y][x] = false;
            prev[y][x] = (Point){-1, -1};
            dist[y][x] = INT_MAX;
            turns[y][x] = ' '; // Initialize turns to space (no turn)
        }
    }

    // Set the start node to visited and distance to 0
    enqueue(&queue, (Point){start_x, start_y});
    visited[start_y][start_x] = true;
    dist[start_y][start_x] = 0; 

    // Run if the queue is not empty
    while (!is_empty(&queue)) {
        Point u = dequeue(&queue);
 
        // If we have reached the destination, this would be the shortest path based on BFS
        if (u.x == dest_x && u.y == dest_y) {
            *shortest_length = dist[u.y][u.x]; 
            break;
        }

        // Checking new neighbors
        for (int i = 0; i < 4; i++) {
            int new_x = u.x + dx[i];
            int new_y = u.y + dy[i];

            // If the new move is valid, update arrays and add it to the queue
            if (is_valid_move(new_x, new_y, map, visited)) {
                visited[new_y][new_x] = true;
                prev[new_y][new_x] = u;
                dist[new_y][new_x] = dist[u.y][u.x] + 1; 
                enqueue(&queue, (Point){new_x, new_y});

                // Check if this move creates a turn
                if ((new_x != start_x) || (new_y != start_y)) {
                    // Determine the turn type based on the previous position
                    if (new_x != u.x && new_y != u.y) {
                        turns[new_y][new_x] = 'T'; // Indicate a turn (diagonal or junction)
                    } else if (new_x != u.x) {
                        turns[new_y][new_x] = 'H'; // Horizontal move
                    } else {
                        turns[new_y][new_x] = 'V'; // Vertical move
                    }
                }
            }
        }
    }

    // Update the path array with the walls
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            path[y][x] = map[y][x];
        }
    }
    
    print_path(prev, (Point){start_x, start_y}, (Point){dest_x, dest_y}, path, turns);
}

static void print_path(Point prev[15][19], Point start, Point end, unsigned char path[15][19], char turns[15][19]) {
    Point current = end;
    while (current.x != start.x || current.y != start.y) {
        path[current.y][current.x] = 9;
        current = prev[current.y][current.x];
    }
    path[start.y][start.x] = 2;

    // Print the nature of the turns along the path
    printf("Nature of turns along the path:\n");
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            if (path[y][x] == 9) {
                printf("%c ", turns[y][x]);
            } else {
                printf(". "); // Indicate no turn
            }
        }
        printf("\n");
    }
}
