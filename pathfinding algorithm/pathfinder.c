#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  // For abs() function

#define ROWS 15
#define COLS 19

// Maze structure
unsigned char map[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1},
    {1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// Start position and food list
unsigned char start_pos[2] = {1,1};
unsigned char food_list[5][2]= {
    {1,9}, {5,5}, {7,1}, {13,5}, {9,9}
};

// Directions for BFS (up, right, down, left)
int directions[4][2] = {
    {-1, 0}, // Up
    {0, 1},  // Right
    {1, 0},  // Down
    {0, -1}  // Left
};

// Output arrays
char turns_nature_array[100];   // Array storing all the turns taken ('U', 'R', 'D', 'L')
int total_turns_array[5];       // Number of total turns to each point
int units_to_point_array[5];    // Units from the final turn to each point
char turn_direction_array[5];   // 'V' for vertical, 'H' for horizontal

// Node structure for BFS queue
typedef struct {
    int row;
    int col;
    int dist;
    int turns;
    char last_direction;
    int last_turn_row;
    int last_turn_col;
} Node;

// BFS function to find the shortest path and track turns
bool is_valid(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && map[row][col] == 0;
}

void bfs(int start_r, int start_c, int end_r, int end_c, int food_index) {
    bool visited[ROWS][COLS] = {false};
    Node queue[ROWS * COLS];
    int front = 0, back = 0;
    
    // Initialize BFS with the start node
    queue[back++] = (Node){start_r, start_c, 0, 0, ' ', start_r, start_c};
    visited[start_r][start_c] = true;
    
    int turn_nature_index = 0;
    
    while (front < back) {
        Node current = queue[front++];
        
        // Check if the current node is the destination
        if (current.row == end_r && current.col == end_c) {
            total_turns_array[food_index] = current.turns;
            
            // Calculate units from the final turn to the node
            units_to_point_array[food_index] = abs(current.row - current.last_turn_row) + abs(current.col - current.last_turn_col);
            
            // Determine if the final turn was vertical or horizontal
            turn_direction_array[food_index] = (current.last_direction == 'U' || current.last_direction == 'D') ? 'V' : 'H';
            return;
        }
        
        // Explore all 4 directions
        for (int i = 0; i < 4; i++) {
            int new_r = current.row + directions[i][0];
            int new_c = current.col + directions[i][1];
            char new_direction = (i == 0) ? 'U' : (i == 1) ? 'R' : (i == 2) ? 'D' : 'L';
            
            if (is_valid(new_r, new_c) && !visited[new_r][new_c]) {
                bool is_turn = (new_direction != current.last_direction && current.last_direction != ' ');
                int last_turn_row = is_turn ? current.row : current.last_turn_row;
                int last_turn_col = is_turn ? current.col : current.last_turn_col;
                
                // Add the new node to the queue
                queue[back++] = (Node){new_r, new_c, current.dist + 1, current.turns + (new_direction != current.last_direction), new_direction, last_turn_row, last_turn_col};
                visited[new_r][new_c] = true;
                
                // Store the turn nature ('U', 'R', 'D', 'L')
                if (new_direction != current.last_direction && current.last_direction != ' ') {
                    turns_nature_array[turn_nature_index++] = new_direction;
                }
            }
        }
    }
}

// Main function
int main() {
    for (int i = 0; i < 5; i++) {
        bfs(start_pos[0], start_pos[1], food_list[i][0], food_list[i][1], i);
        printf("Total turns to point %d: %d\n", i + 1, total_turns_array[i]);
        printf("Units to final point %d: %d\n", i + 1, units_to_point_array[i]);
        printf("Final turn direction: %c\n", turn_direction_array[i]);
    }

    // Print the full array of turns taken
    printf("\nNature of turns taken:\n");
    for (int i = 0; turns_nature_array[i] != '\0'; i++) {
        printf("%c ", turns_nature_array[i]);
    }
    printf("\n");

    return 0;
}
