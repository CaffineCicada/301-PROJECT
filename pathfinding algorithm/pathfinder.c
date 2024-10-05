#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 15
#define COL 19
#define MAX_QUEUE_SIZE 1000
#define MAX_TURNS 100 // Maximum number of turns to record

unsigned char map[ROW][COL] = {
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

typedef struct {
    int x, y;
    int prev_direction; // Store the previous direction to detect turns
    int distance;       // Store the distance from the start
} Node;

typedef struct {
    Node queue[MAX_QUEUE_SIZE];
    int front, rear;
} Queue;

// Function to initialize the queue
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

// Function to check if the queue is empty
int isQueueEmpty(Queue *q) {
    return q->front == q->rear;
}

// Function to enqueue a node
void enqueue(Queue *q, Node n) {
    if (q->rear < MAX_QUEUE_SIZE) {
        q->queue[q->rear++] = n;
    }
}

// Function to dequeue a node
Node dequeue(Queue *q) {
    return q->queue[q->front++];
}

// Directions for moving in the grid (0: Up, 1: Right, 2: Down, 3: Left)
int directions[4][2] = {
    {-1, 0},  // Up
    {0, 1},   // Right
    {1, 0},   // Down
    {0, -1}   // Left
};

// Function to get the nature of the turn
const char* getTurn(int prev_dir, int new_dir) {
    if (prev_dir == new_dir) return "Straight";
    if ((prev_dir + 1) % 4 == new_dir) return "Right";
    if ((prev_dir + 3) % 4 == new_dir) return "Left";
    return "U-turn"; // If you go back to the previous direction
}

void bfs(unsigned char start[2]) {
    int visited[ROW][COL] = {0};
    char turn_history[ROW][COL][MAX_TURNS][10]; // To store the nature of turns taken

    // Initialize turn history to empty strings
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            for (int k = 0; k < MAX_TURNS; k++) {
                turn_history[i][j][k][0] = '\0'; // Empty string
            }
        }
    }

    Queue q;
    initQueue(&q);

    Node startNode = {start[0], start[1], -1, 0}; // Initialize start node with no previous direction and distance 0
    enqueue(&q, startNode);
    visited[start[0]][start[1]] = 1;

    while (!isQueueEmpty(&q)) {
        Node current = dequeue(&q);

        // Check if we reached a food location
        for (int i = 0; i < 5; i++) {
            if (current.x == food_list[i][0] && current.y == food_list[i][1]) {
                printf("Reached food at (%d, %d) with distance %d\n", current.x, current.y, current.distance);
                
                // Print the nature of the turns taken to reach this node
                printf("Turns taken to reach (%d, %d): ", current.x, current.y);
                int turn_count = 0; // Count of turns taken
                for (int j = 0; j < current.distance; j++) {
                    if (turn_history[current.x][current.y][j][0] != '\0') {
                        if (turn_count > 0) {
                            printf(", "); // Add comma for separation
                        }
                        printf("%s", turn_history[current.x][current.y][j]);
                        turn_count++;
                    }
                }
                printf("\n");
                break; // Break if food is reached
            }
        }

        // Explore all possible directions
        for (int i = 0; i < 4; i++) {
            int newX = current.x + directions[i][0];
            int newY = current.y + directions[i][1];

            // Check boundaries and if the cell is not a wall
            if (newX >= 0 && newX < ROW && newY >= 0 && newY < COL && map[newX][newY] == 0 && !visited[newX][newY]) {
                visited[newX][newY] = 1;

                Node newNode = {newX, newY, i, current.distance + 1}; // Update distance for the new node
                enqueue(&q, newNode);

                // Check if the current position is a junction (has multiple valid directions)
                int valid_moves = 0;
                for (int j = 0; j < 4; j++) {
                    int adjX = newX + directions[j][0];
                    int adjY = newY + directions[j][1];
                    if (adjX >= 0 && adjX < ROW && adjY >= 0 && adjY < COL && map[adjX][adjY] == 0 && (adjX != current.x || adjY != current.y)) {
                        valid_moves++;
                    }
                }

                // Record the nature of the turn if it's a junction
                if (valid_moves > 1 && current.prev_direction != -1) {
                    int turn_index = current.distance; // index of the turn in the history
                    strcpy(turn_history[newX][newY][turn_index], getTurn(current.prev_direction, i));
                }
            }
        }
    }
}

int main() {
    bfs(start_pos);
    return 0;
}
