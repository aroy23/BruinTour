# BruinTour

BruinTour is a tour guide system designed to provide automated walking tours of UCLA and Westwood for visiting students and parents. It leverages geospatial data from Open Street Map (OSM) to generate instructions for tour robots, enabling them to navigate to various landmarks and deliver commentary along the way.

## Input Files

The input to the BruinTour system consists of two files:

1. **mapdata.txt**: This data file contains details on over 60,000 of streets in Westwood. It lists the latitude and longitude of each street block by block, derived from the Open Street Map service. Additionally, the file contains dozens of "points of interest" (POIs) that are candidates for the tour. These POIs include landmarks such as Diddy Riese, Kerckhoff Hall, etc.

2. **stops.txt**: This file contains a proposed list of points of interest (POIs) for a given tour, along with a description of each POI for the robot to vocalize. For example:

- Ackerman Union|This is Ackerman where you buy stuff.
- John Wooden Center|This is where you go to get swole.
- Diddy Riese|They sell yummy cheap cookies here.

## Output Instructions

The output of the BruinTour system is a set of instructions for a BruinTour robot to follow in order to give its tour. These instructions fall into three different categories:

- **Commentary**: Instructs the robot to give commentary about the current POI. For example: "John Wooden Center: This is where you get swole."

- **Proceed**: Instructs the robot to proceed in a particular direction on a street.

- **Turn**: Instructs the robot to make a turn from one street onto another.

## Example Output

The provided `main.cpp` file will take the tour instructions created by your classes and print something like this:

# Tour Route

- Starting tour...
- Welcome to Ackerman Union!
- This is Ackerman where you buy stuff.
- Proceed 0.028 miles north on a path
- Take a left turn on Bruin Walk
- Proceed 0.098 miles west on Bruin Walk
- Take a right turn on a path
- Proceed 0.074 miles north on a path
- Welcome to John Wooden Center!
- This is where you go to get swole.
- Proceed 0.074 miles south on a path
- Take a left turn on Bruin Walk
- Proceed 0.043 miles east on Bruin Walk
- Take a right turn on Westwood Plaza
- Proceed 0.514 miles south on Westwood Plaza
- Take a right turn on Le Conte Avenue
- Proceed 0.097 miles west on Le Conte Avenue
- Take a left turn on Broxton Avenue
- Proceed 0.053 miles south on Broxton Avenue
- Take a left turn on a path
- Proceed 0.015 miles northeast on a path
- Welcome to Diddy Riese!
- They sell yummy cheap cookies here.
- Your tour has finished!
- Total tour distance: 0.996 miles

This output provides detailed instructions for the person to navigate through the tour stops, along with commentary about each point of interest. At the end of the tour, it displays the total tour distance.

## Components

### 1. Geographic Database

The `GeoDatabase` class is responsible for:
- Loading OSM map data from a text file.
- Indexing street segments and POIs for efficient lookup.
- Providing functions to retrieve neighboring GeoPoints, locate POIs, and determine street names.

### 2. Router

The `Router` class computes routes between two GeoPoints using a routing algorithm. It utilizes the Geographic Database to find the shortest path and returns a vector of GeoPoints representing the route.

### 3. Tour Generator

The `TourGenerator` class utilizes the Geographic Database and Router to generate tour instructions for a given set of PoIs. It produces a vector of tour commands, including commentary, proceed commands, and turn commands.

### 4. HashMap

The `HashMap` class is a template class that implements an open hash table. It associates strings with arbitrary values and automatically grows its number of buckets to maintain a specified load factor.

## Usage

To use the BruinTour system:
1. Compile the source code with the provided `main.cpp` file.
2. Execute the compiled program with paths to the map data file (`mapdata.txt`) and the stops file (`stops.txt`) as command-line arguments.

Example:

 ./BruinTour mapdata.txt stops.txt
 
The program will load the map data, generate tour instructions based on the provided stops, and print them to the screen.

# A* Algorithm Implementation in BruinTour

The A* algorithm plays a crucial role in the BruinTour system's Router class for computing efficient routes between two GeoPoints on the map. Below, we'll discuss the implementation of the A* algorithm within the Router class and its significance in providing optimized navigation for the tour.

## A* Algorithm Overview

The A* algorithm is a popular pathfinding algorithm known for its efficiency and optimality in finding the shortest path between two points on a graph. It combines elements of both uniform-cost search and greedy best-first search by considering both the cost of reaching a node from the starting point (known as the "g-value") and the estimated cost of reaching the destination from the node (known as the "h-value").

### Key Components of A* Algorithm:

1. **Heuristic Function**: A* uses a heuristic function to estimate the cost of reaching the destination from any given node. In the context of BruinTour, the heuristic function may estimate the straight-line distance (Euclidean distance) between the current node and the destination node.

2. **Open and Closed Lists**: A* maintains two lists: the open list and the closed list. The open list contains nodes that are yet to be explored, while the closed list contains nodes that have already been visited.

3. **Priority Queue**: A* employs a priority queue to efficiently select the next node to explore. Nodes with lower total cost (f-value) are given higher priority in the queue.

## Implementation Details

In the BruinTour system, the A* algorithm is implemented within the `Router` class, which computes routes between two GeoPoints. Here are some critical implementation details:

- **Adherence to Base Class**: The `Router` class is derived from the provided `RouterBase` class, ensuring compliance with the specified interface and requirements.

- **Utilization of GeoDatabase**: The `Router` class utilizes the `GeoDatabaseBase` class to access map data and stitch together a set of waypoints (GeoPoints) between the starting and ending GeoPoints.

- **Efficiency**: The A* implementation in the `Router` class ensures that the route-finding algorithm runs in O(N*logN) time or less, where N represents the number of GeoPoints in the mapping data.

## Benefits of A* Algorithm in BruinTour

- **Efficient Route Finding**: A* ensures efficient computation of routes, enabling tour robots to navigate through the map in a timely manner.

- **Optimal Paths**: By considering both actual and estimated costs, A* guarantees to find the shortest path from the starting point to the destination.

- **Adaptability**: A* can adapt to various map layouts and environments, providing versatile navigation capabilities for the BruinTour system.

## Conclusion

The A* algorithm serves as a cornerstone in the BruinTour system's `Router` class, offering an efficient and optimal solution for route computation. By leveraging A*, the system ensures smooth navigation for tour robots, enhancing the overall tour experience for visitors exploring UCLA and Westwood.

For implementation details of the Router class and A* algorithm, please refer to the source code in the repository.

This Program Was Made For UCLA CS 32 Project 4
