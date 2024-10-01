// note, chatgpt and geeksforgeeks was used to help with understanding this algorithm although i would say that my
// understanding still isnt the best
#include "astar.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 1000

// used to refer to a point in the maze
typedef struct {
    int x, y;
} Point;

// Structure to represent a node in the A* algorithm
typedef struct {
    Point point; // The position of the node
    int g; // Cost from the start node to this node
    int h; // Heuristic cost estimate to the goal
    int f; // Total cost (g + h)
} Node;

// Priority queue is used to explore nodes
typedef struct {
    Node nodes[MAX_QUEUE_SIZE];
    int size;
} PriorityQueue;

// Function to swap two nodes in the priority queue
static void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

// this is used to put a node to the priority queue
static void push(PriorityQueue* pq, Node node) {
    int i = pq->size++;
    // Place the new node in its correct position based on its f-cost
    while (i > 0 && node.f < pq->nodes[(i - 1) / 2].f) {
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->nodes[i] = node;
}

// Function to pop the node with the lowest f-cost from the priority queue
static Node pop(PriorityQueue* pq) {
    Node root = pq->nodes[0];
    Node last = pq->nodes[--pq->size];
    int i = 0;
    int child;
    // Reheapify the priority queue
    while ((child = 2 * i + 1) < pq->size) {
        if (child + 1 < pq->size && pq->nodes[child + 1].f < pq->nodes[child].f) {
            child++;
        }
        if (last.f <= pq->nodes[child].f) break;
        pq->nodes[i] = pq->nodes[child];
        i = child;
    }
    pq->nodes[i] = last;
    return root;
}

static bool is_valid_move(int x, int y, unsigned char map[15][19], bool visited[15][19]) {
    return x >= 0 && x < 19 && y >= 0 && y < 15 && map[y][x] != 1 && !visited[y][x];
}

// Function to calculate the heuristic (Manhattan distance) between two points
static int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// used to update the map
static void print_path(Point prev[15][19], Point start, Point end, unsigned char path[15][19]) {
    Point current = end;
    while (current.x != start.x || current.y != start.y) {
        path[current.y][current.x] = 9; 
        current = prev[current.y][current.x];
    }
    path[start.y][start.x] = 2; 
}

void astar(unsigned char map[15][19], unsigned char start_x, unsigned char start_y, unsigned char dest_x, unsigned char dest_y, unsigned char path[15][19], int *shortest_length) {
    PriorityQueue open_list = {{0}, 0}; // Initialize the priority queue
    bool visited[15][19]; // Array to keep track of visited nodes
    Point prev[15][19]; // Array to keep track of the previous node for each node (for path reconstruction)
    // possible x and y movements
    int dx[] = {0, 0, -1, 1}; 
    int dy[] = {-1, 1, 0, 0}; 
    
    // Initialize the visited and prev arrays
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            visited[y][x] = false;
            prev[y][x] = (Point){-1, -1};
        }
    }

    // Create and push the start node onto the priority queue
    Node start_node = { {start_x, start_y}, 0, heuristic(start_x, start_y, dest_x, dest_y), 0 };
    push(&open_list, start_node);
    visited[start_y][start_x] = true;

    // Main loop to explore the nodes
    while (open_list.size > 0) {
        Node current_node = pop(&open_list); // Get the node with the lowest f-cost
        Point u = current_node.point;

        // if we have reached the end node, updated the shortest path
        if (u.x == dest_x && u.y == dest_y) {
            *shortest_length = current_node.g;
            break;
        }

        // Explore the neighbors of the current node
        for (int i = 0; i < 4; i++) {
            int new_x = u.x + dx[i];
            int new_y = u.y + dy[i];

            // If the move is valid, calculate the costs and push the neighbor onto the priority queue
            if (is_valid_move(new_x, new_y, map, visited)) {
                int new_g = current_node.g + 1; // Cost from start to this neighbor
                int new_h = heuristic(new_x, new_y, dest_x, dest_y); // Heuristic cost to goal
                int new_f = new_g + new_h; // Total cost

                Node neighbour_node = { {new_x, new_y}, new_g, new_h, new_f };
                if (!visited[new_y][new_x]) {
                    visited[new_y][new_x] = true; // Mark the neighbor as visited
                    prev[new_y][new_x] = u; // Set the previous node for path reconstruction
                    push(&open_list, neighbour_node); // Push the neighbor onto the priority queue
                }
            }
        }
    }

    // copies the map into path
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 19; x++) {
            path[y][x] = map[y][x];
        }
    }

    print_path(prev, (Point){start_x, start_y}, (Point){dest_x, dest_y}, path);
}
