Pathfinder
==========

C++

Models the graph, a recursive data structure consisting of a collection of nodes, each of which can have any number of links (called arcs) to other nodes.

1. **Optimal search path (Dijkstra's algorithm)**
Organizes the search to pursue the promising short paths first and only explore the longer routes if nothing better
is found earlier. Does this by keeping a priority queue of all paths constructed, prioritized in terms of distance.

2. **Minimal spanning tree (Kruskal's algorithm)**
Finds the spanning tree in which the total cost associated with the arcs is as small as possible. Goal is to minimize the overall total distance. Considers shorter arcs before the longer ones (uses a priority queue as well).


From the user’s point of view, the program draws a graph and provides two operations: finding the shortest path between two locations and constructing the minimal cost network. The user chooses the graph data file for the program to read and display.

The program offers the user a menu of choices:
 (1) Choose a new graph data file
 (2) Find shortest path using Dijkstra's algorithm
 (3) Compute the minimal spanning tree using Kruskal's algorithm
 (4) Quit
 
If the user chooses the shortest path search, the program prompts the user to click on two locations. The program finds the shortest path connected the two and displays it. If the user chooses the minimal spanning tree option, the program determines the set of links that connect all locations with the minimal overall cost and displays them. The user can do additional searches and/or change data files. The program exits when the user chooses to quit.
