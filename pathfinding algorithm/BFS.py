from collections import deque

def algorithm(maze, start, end):
    queue = deque([(start, [start])])
    visited = set()

    while queue:
        # once we have reached to the destination
        current, path = queue.popleft()
        if current == end:
            return path
        
        # this is to avoid visiting the same cell again
        visited.add(current)
        
        # same with dfs, we check all the neighbours
        for direction in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
            neighbour = (current[0] + direction[0], current[1] + direction[1])
            # checks if the neighbour is within the maze and not a wall
            if 0 <= neighbour[0] < len(maze[1]) and 0 <= neighbour[1] < len(maze):
                # checks if the neighbour has not been visited and then add to the queue
                if maze[neighbour[1]][neighbour[0]] != 1 and neighbour not in visited:
                    queue.append((neighbour, path + [neighbour]))
                    visited.add(neighbour)
    
    return None
