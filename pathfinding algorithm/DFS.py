def algorithm(maze, start, end):
    stack = [(start, [start])]
    shortest_path = None

    # here is the main algorithm
    while stack:
        (current, path) = stack.pop()

        # once we reach the end, we want to see if this is the shortest path and update if true
        if current == end:
            if shortest_path is None or len(path) < len(shortest_path):
                shortest_path = path

        # look at north, south, east, west directions to see if the next space is valid
        for direction in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
            neighbour = (current[0] + direction[0], current[1] + direction[1])
            # if the neighbour is part of the path and hasnt been visited yet, add it to the stack
            if 0 <= neighbour[0] < len(maze[1]) and 0 <= neighbour[1] < len(maze):
                if maze[neighbour[1]][neighbour[0]] != 1 and neighbour not in path:
                    stack.append((neighbour, path + [neighbour]))

    return shortest_path



