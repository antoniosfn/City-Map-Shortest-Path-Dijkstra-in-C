# City Map Shortest Path Dijkstra in C

This C language project implements a system to find the shortest path between two points on a city map, considering only street intersections as points of interest. The solution leverages Dijkstra's Algorithm to determine the most efficient routes.

The city map is modeled as a graph, where intersections are represented as nodes and street segments as edges. Distances between intersections are calculated using provided Cartesian coordinates. Traffic flow directions on each street are respected, as indicated by arrows on the map and within the graph representation. 

Key Features:

Graph Representation: The map is stored as a graph, with nodes for intersections and directed edges for street segments, respecting traffic flow. 

Data Loading: The program reads map data from binary files (pre-generated from text files like points.txt and neighbors.txt), which contain information about points (ID, coordinates, street names) and connections between them. 
Distance Calculation: Distances between neighboring points (edges) are determined by calculating their Cartesian coordinates. 
Shortest Path Algorithm: Utilizes Dijkstra's Algorithm to find the shortest path between a user-defined origin and destination. 

Detailed Route Report: Generates an interactive report detailing the sequence of movements the user should make to complete the journey, including instructions like "Go straight on a certain street until a certain point" and "Turn left (or right) at the intersection with a certain street." 
Interactivity: The user interacts with the program via a menu to select the origin and destination of the route. 
Modularity: Common type definitions and functions are organized into separate .h (header) and .c (implementation) files for better structure. 
This project demonstrates fundamental concepts of graph algorithms and data manipulation in C to solve a practical route optimization problem.
