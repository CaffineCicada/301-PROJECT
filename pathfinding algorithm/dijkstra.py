import heapq  

def algorithm(maze, start, end):
    neighbours = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    vistied = set()
    came_from = {}
    
    # keeps track of the shortest distance between the start node to other nodes
    gscore = {start: 0}
    
    # using a heap queue
    oheap = []
    heapq.heappush(oheap, (gscore[start], start))
    
    # loop until queue is empty
    while oheap:
        current = heapq.heappop(oheap)[1]
        
        # we have reached the end node
        if current == end:
            data = []
            # reconstruct the path using came_from
            while current in came_from:
                data.append(current)
                current = came_from[current]
            data.append(start)
            # Return the path in the correct order (from start to end)
            return data[::-1]
        
        vistied.add(current)
        
        # Check all possible movements
        for i, j in neighbours:
            neighbour = current[0] + i, current[1] + j
            tentative_g_score = gscore[current] + 1  
            
            # check if the neighbouring node is a possible path
            if 0 <= neighbour[0] < len(maze[0]): 
                if 0 <= neighbour[1] < len(maze):  
                    if maze[neighbour[1]][neighbour[0]] == 1:  
                        continue
                else:
                    continue
            else:
                continue
                
            # if we have visited the neighbour but the path isnt shorter, we skip it
            if neighbour in vistied and tentative_g_score >= gscore.get(neighbour, 0):
                continue
                
            # If the new path to the neighbor is shorter or the neighbor is not in the heap
            if tentative_g_score < gscore.get(neighbour, 0) or neighbour not in [i[1] for i in oheap]:
                came_from[neighbour] = current  
                gscore[neighbour] = tentative_g_score 
                # Add the neighbor to the heap with its g-score as the priority
                heapq.heappush(oheap, (gscore[neighbour], neighbour))
                
    # if no path is found
    return False
