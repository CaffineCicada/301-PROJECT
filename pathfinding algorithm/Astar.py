// note, chatgpt and geeksforgeeks was used to help with understanding this algorithm although i would say that my
// understanding still isnt the best
import heapq

# Heuristic function: This function estimates the cost to reach the goal from node 'a'.
# Here, it uses the Manhattan distance (good for grids where you can only move in 4 directions).
def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

# A* search algorithm to find the shortest path in a maze from 'start' to 'end'.
def algorithm(maze, start, end):
    neighbours = [(0, 1), (1, 0), (0, -1), (-1, 0)]  # Possible moves: right, down, left, up
    close_set = set()  # Set of nodes that have already been evaluated
    came_from = {}  # Dictionary to track the best previous node for each node
    gscore = {start: 0}  # Dictionary to track the cost from start to a node
    fscore = {start: heuristic(start, end)}  # Dictionary to track the estimated total cost from start to end
    oheap = []  # Priority queue (min-heap) to select the next node to explore
    
    # Start by pushing the starting node into the heap, with its fscore as the priority
    heapq.heappush(oheap, (fscore[start], start))
    
    # Main loop: keep processing nodes until there are none left to explore
    while oheap:
        # Pop the node with the lowest fscore from the heap (i.e., the most promising node)
        current = heapq.heappop(oheap)[1]
        
        # If we've reached the end, reconstruct the path by tracing back through 'came_from'
        if current == end:
            data = []  # List to store the path
            while current in came_from:  # Keep moving backwards through the path
                data.append(current)
                current = came_from[current]
            data.append(start)  # Don't forget to add the start node
            return data[::-1]  # Reverse the path to get it from start to end
        
        close_set.add(current)  # Mark the current node as evaluated (closed)
        
        # Explore each of the neighboring nodes
        for i, j in neighbours:
            neighbour = current[0] + i, current[1] + j  # Calculate the neighbor's position
            tentative_g_score = gscore[current] + 1  # Calculate the cost to reach this neighbor
            
            # Check if the neighbor is within the maze bounds
            if 0 <= neighbour[0] < len(maze[0]):
                if 0 <= neighbour[1] < len(maze):
                    if maze[neighbour[1]][neighbour[0]] == 1:  # Check if the neighbor is a wall
                        continue  # Skip if it's a wall
                else:
                    continue  # Skip if the neighbor is out of bounds vertically
            else:
                continue  # Skip if the neighbor is out of bounds horizontally
                
            # If this path is not better than any previously found, skip it
            if neighbour in close_set and tentative_g_score >= gscore.get(neighbour, 0):
                continue
                
            # If this path is better, or the neighbor hasn't been explored, update it
            if tentative_g_score < gscore.get(neighbour, 0) or neighbour not in [i[1] for i in oheap]:
                came_from[neighbour] = current  # Record the best previous node
                gscore[neighbour] = tentative_g_score  # Update the gscore for this node
                fscore[neighbour] = tentative_g_score + heuristic(neighbour, end)  # Update the fscore
                heapq.heappush(oheap, (fscore[neighbour], neighbour))  # Push the neighbor to the heap
                
    return False  # If no path is found, return False
