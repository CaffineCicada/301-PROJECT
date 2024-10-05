import pygame
 
import BFS



# Initialize Pygame
pygame.init()

TILE_SIZE = 20
GRID_WIDTH = 19
GRID_HEIGHT = 15
WIDTH, HEIGHT = GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE + 40  # Increased height for text

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)

# Create the screen
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Maze Visualization")

# Define the maze (15x19) with 5 premade nodes labeled as -1 and a starting node labeled as -2
map = [
    [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
	[1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1],
	[1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1],
	[1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1],
	[1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1],
	[1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1],
	[1,0,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1],
	[1,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1],
	[1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1],
	[1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1],
	[1,1,1,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1],
	[1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1],
	[1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1],
	[1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1],
	[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
]

# Define premade nodes
food_list = [(5,5),
	(1,9),
	(7,1),
	(13,5),
	(9,9)]
current_node_index = 0

# Place premade nodes in the maze
for x, y in food_list:
    map[y][x] = -1

# Define the starting node
start_pos = (1, 1)

# Place the starting node in the maze
map[start_pos[1]][start_pos[0]] = -2

def draw_maze(screen, maze):
    for y, row in enumerate(maze):
        for x, tile in enumerate(row):
            if tile == 1:
                color = BLACK
            elif tile == 0:
                color = WHITE
            elif tile == -1:
                color = RED
            elif tile == -2:
                color = GREEN
            elif tile == 2:
                color = BLUE
            pygame.draw.rect(screen, color, (x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE))

def clear_path(maze):
    for y, row in enumerate(maze):
        for x, tile in enumerate(row):
            if tile == 2:
                maze[y][x] = 0

def main():
    global start_pos, current_node_index
    clock = pygame.time.Clock()
    running = True
    path_found = False
    path_length = 0

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:
                    if path_found:
                        clear_path(map)
                        current_node_index += 1
                        if current_node_index >= len(food_list):
                            # current_node_index = 0 
                            running = False
                            break

                    end_node = food_list[current_node_index]
                    
                    # only change the file import to change the algorithm
                    path = BFS.algorithm(map, start_pos, end_node)

                    if path:
                        path_length = len(path) - 1  # excluding the start node itself
                        for (x, y) in path:
                            map[y][x] = 2
                        map[end_node[1]][end_node[0]] = -2
                        map[start_pos[1]][start_pos[0]] = 0
                        start_pos = end_node
                    path_found = True

        screen.fill(WHITE)
        draw_maze(screen, map)

        # Display path length
        font = pygame.font.Font(None, 36)
        text = font.render(f"Path length: {path_length}", True, BLACK)
        screen.blit(text, (10, HEIGHT - 30))

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()

if __name__ == "__main__":
    main()