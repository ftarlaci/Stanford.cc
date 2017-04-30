/**
Author: Fatma Tarlaci
* This program implements some of the most known graph
* searching algorithms including Breatdh-First Search,
* Dijkstra's Algorithm, and A* Search, as well as another
* function to find routes alternative to the shortest paths
* we can find through Dijkstra's and A*.
*/

#include "Trailblazer.h"
#include "queue.h"
#include "Color.h"
#include "trailblazer.h"
#include "set.h"
#include "pqueue.h"
#include "RoadGraph.h"
#include "strlib.h"
#include "vector.h"
#include "console.h"
#include "filelib.h"
#include "hashmap.h"
#include "map.h"

using namespace std;

/** Function prototypes */
Path aStarModified(RoadNode * start, RoadNode * end, RoadEdge * toIgnore);
double uniqueness(Path mainPath, Path newPath);



/**
 * breadthFirstSearch() implements a breadth-first search  (BFS) algorithm
 * to find a path in a given node, and start and end nodes. We use Priority
 * queue of Path's (defined as Vector of nodes) to implement our function,
 * so that we can return the shortest path from the sorted queue. The color
 * coding is implemented for visual ques to follow the implementation of the
 * algorithm in the graph.
*/
Path breadthFirstSearch(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    Queue<Path>  pathQueue;             /** create a queue of paths */
    Set<RoadNode *> green;              /** create a set of green nodes */
    Path currentPath;                   /** creating a variable to keep track of the current path. */
    RoadNode * currentNode = start;     /** creating a variable to keep track of the current node. */

    pathQueue.enqueue({start});         /** enqueue the start node as the first path */
    start->setColor(Color::YELLOW);     /** color the start node yellow */


    while(!pathQueue.isEmpty()){
        currentPath = pathQueue.dequeue();
        RoadNode *currentNode = currentPath[currentPath.size()-1];
        currentNode->setColor(Color::GREEN);            /** set it to green and put in the set of green nodes. */
        green.add(currentNode);


        if(currentNode == end) {                        /** if a path found, break out of the loop here. */
            return currentPath;

        }

        /** If no path is found, we will keep looping using each of the negihbors of our current node. */
        for(RoadNode * neighbor : graph.neighborsOf(currentNode)){
            if(!green.contains(neighbor)){
                neighbor->setColor(Color::YELLOW);
                Path newPath = currentPath;
                newPath.add(neighbor);
                pathQueue.enqueue(newPath);
            }
        }
    }
    return {};
} /** end of breadthFirstSearch() */


/**
 * Dijkstra's algorithm is another graphs earching algorithm, which is slightly
 * different than BFS. In Dijkstra's each edge between nodes are weighted and we
 * are guaranteed to find the smallest weight path (which is the shortest. While
 * in the BFS guarantees to return a path, Dijkstra's guarantees to return the
 * shortest path. Our function takes in @param a graph, @param start node and
 * @param end node and returns the shortest path between them based on the
 * weights associated with the edges.
*/

Path dijkstrasAlgorithm(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    PriorityQueue<Path> dijkstrasPQ;                    /** create a priority queue of paths */
    Set<RoadNode *> green;                              /** create a set for greened nodes */
    Path currentPath;                                   /** create a variable for current path */
    RoadEdge * edge;
    RoadNode * currentNode = start;                     /** create a node variable to keep track of current node. */


    dijkstrasPQ.enqueue({start}, 0.0);                  /** enqueue start node at distance zero. */
    start->setColor(Color::YELLOW);                     /** color the node yellow */


    if(currentNode == end) {                            /** If start and end nodes are the same return start. */
        return {start};
    }

    while(!dijkstrasPQ.isEmpty()){
        double pathCost = dijkstrasPQ.peekPriority();   /** Peek the priority so that we can calculate the pathcost */
        currentPath = dijkstrasPQ.dequeue();            /** dequeue the current node. */
        currentNode = currentPath[currentPath.size()-1]; /** Update the current node */
        if(!green.contains(currentNode)) {
            currentNode->setColor(Color::GREEN);        /** color the node green */
            green.add(currentNode);                     /** add it to the green land */
            if(currentNode == end) {
                return currentPath;
            }
            for(RoadNode * neighbor :  graph.neighborsOf(currentNode)){
                if(!green.contains(neighbor)){
                    currentNode->setColor(Color::YELLOW);
                    Path newPath = currentPath;
                    newPath.add(neighbor);
                    edge = graph.edgeBetween(currentNode, neighbor);
                    dijkstrasPQ.enqueue(newPath, edge->cost() + pathCost);
                }
            }
        }
    }
    return {};                                  /** With no path to return; we return an empty path */
} /** end of Dijkstra's algorithm */

/**
 * A* graph search algorithm is relatively more efficient than Dijkstra's
 * Algorithm as it keeps track of distance at each step to track if the step
 * taken is getting closer to the target point where we want to arrive through
 * our searching algorithm. A* implements a heuristic distance by
 * crowFlyDistanceBetween(currentNode, end) function that we are given we are
 * provided and the maximum speed allowed on that specific route. A* provides a
 * closer implementation of a real world navigation search by considering such
 * details. Our function follows a similar logic to Dijkstra's Algorithm with
 * the addition of the heuristic detail.
*/
Path aStar(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    PriorityQueue<Path> aStarPQ;                        /** create a priority queue of paths */
    Set<RoadNode *> green;                              /** create a set for greened nodes */
    Path currentPath;                                   /** create a variable for current path */
    RoadEdge * edge;
    RoadNode * currentNode = start;                     /** create a node variable to keep track of current node. */
    double  theHeuristic;

    aStarPQ.enqueue({start}, 0.0);                      /** enqueue start node at distance zero */
    start->setColor(Color::YELLOW);                     /** color the node yellow */

    if(currentNode == end) {                            /** If start and end nodes are the same return start. */
        return {start};
    }


    /** While our priority queue is not empty we keep enqueuing
     * and dequeuing the nodes to find the shortest path. meanwhile,
     *  we keep track of the current node
     */
    while(!aStarPQ.isEmpty()){
        currentPath = aStarPQ.dequeue();
        currentNode = currentPath[currentPath.size()-1];
        if(!green.contains(currentNode)) {
            currentNode->setColor(Color::GREEN);
            green.add(currentNode);

            if(currentNode == end) {                    /** if we have arrived at our target point (end) we can stop */
                return currentPath;
            }

            /** If not, we keep trying the neighboring nodes of our current nodes */
            for(RoadNode * neighbor :  graph.neighborsOf(currentNode)){
                if(!green.contains(neighbor)){
                    currentNode->setColor(Color::YELLOW);
                    Path newPath = currentPath;
                    newPath.add(neighbor);
                    edge = graph.edgeBetween(currentNode, neighbor);

                    /** Calculate the heuristic distance */
                    theHeuristic = graph.crowFlyDistanceBetween(currentNode, end) / graph.maxRoadSpeed();

                    double pathCost = 0;   /** Create a variable to store the path cost. */


                    for (int i = 1; i < newPath.size(); i++) {

                        /** the  edgecost between each pair of nodes in the path will be added to the path */
                        pathCost += graph.edgeBetween(newPath[i - 1], newPath[i])->cost();
                    }
                    /** Finally we enqueue the new path that we have found, its cost as well as its heuristic. */
                    aStarPQ.enqueue(newPath, pathCost + theHeuristic);
                }
            }
        }
    }
    return {};  /** With no path to return; we return an empty path */

} /** End of aStar */



/**
 * This helper function is an extended version of the A* function we
 * implemented above. The only difference in this function is that it
 * excldues the actual edge we took in finding shortest path with A*
 * so that we can find alterantive routes by trying different edges
 * during our search. Similarly we create a priority queue for our
*/

Path aStarModified(const RoadGraph& graph, RoadNode * start, RoadNode * end, RoadEdge * toIgnore){
    PriorityQueue<Path> aStarModifiedPQ;
    Set<RoadNode *> green;
    Path currentPath;
    RoadEdge * edge;


    aStarModifiedPQ.enqueue({start}, 0.0);
    start->setColor(Color::YELLOW);
    RoadNode * currentNode = start;
    double  theHeuristic;

    if(currentNode == end) {
        return {start};
    }

    while(!aStarModifiedPQ.isEmpty()){
        currentPath = aStarModifiedPQ.dequeue();
        currentNode = currentPath[currentPath.size()-1];
        if(!green.contains(currentNode)) {
            currentNode->setColor(Color::GREEN);
            green.add(currentNode);
            if(currentNode == end) {
                return currentPath;
            }

            /** If no path found, try the neighboring nodes. */
            for(RoadNode * neighbor : graph.neighborsOf(currentNode)) {
                if(graph.edgeBetween(currentNode, neighbor) != toIgnore) {
                    if(!green.contains(neighbor)){
                        currentNode->setColor(Color::YELLOW);
                        Path newPath = currentPath;
                        newPath.add(neighbor);
                        edge = graph.edgeBetween(currentNode, neighbor);
                        theHeuristic = graph.crowFlyDistanceBetween(currentNode, end) / graph.maxRoadSpeed();
                        double pathCost = 0;
                        for (int i = 1; i < newPath.size(); i++) {

                            /** the  edgecost between each pair of nodes in the path will be added to the path */
                            pathCost += graph.edgeBetween(newPath[i - 1], newPath[i])->cost();
                        }
                        aStarModifiedPQ.enqueue(newPath, pathCost + theHeuristic);
                    }
                }
            }
        }
    }
    return {};
} /** end of aStartModified */


/**
 * In the alternative routes function, we will need to create a feature of
 * uniqueness which will determine the percentage of difference from the main
 * path (the shortest path we found with our A* function. This difference in
 * percentage should be equal or greater than %20. Thus, we create three sets
 * where we store the nodes in our main path, the nodes in the new alternate
 * path that we find, and their intersecting nodes so that we can calculate
 * the difference.
*/
double uniqueness(Path mainPath, Path newPath){
    Set<RoadNode *> mainPathNodes;
    Set<RoadNode *> newPathNodes;
    Set<RoadNode *> intersectingNodes;

    for(RoadNode * node : mainPath) {
      mainPathNodes.add(node);

    }
    for(RoadNode * node : newPath) {
      newPathNodes.add(node);
    }

    /** Create a variable to store the intersection of the two different sets */
    intersectingNodes = newPathNodes * mainPathNodes;

    /** do the calculation to find the uniquness level. */
    double newPathSize = newPathNodes.size();
    double uniquenessPercentage = (newPathSize - intersectingNodes.size()) / newPathSize;
    return uniquenessPercentage;
}



/**
 * alternativeRoute() will return alternative routes to reach to our destination
 * designated by the "end" node. Here, we call our A* function to return the
 * main path so that we can search for alternate routes.
*/
Path alternativeRoute(const RoadGraph& graph, RoadNode* start, RoadNode* end) {

    /** calling the A * function to get the optimal/main path. */
    Path mainPath = aStar(graph, start, end);

    /** create an empy priority queue to store the alternate paths */
    PriorityQueue<Path> alternatePaths;
    double  theHeuristic;
    RoadNode * currentNode = start;


    /**
     * for all the edges used in the main path, we create a new path which
     * will call the modified A* function and ignore that edge which is taken
     * in the A *
     */

    for(int i = 0; i < mainPath.size()-1; i++){
        RoadEdge * toIgnore = graph.edgeBetween(mainPath[i], mainPath[i + 1]);
        Path newPath = aStarModified(graph, start, end, toIgnore);
        double uniquenessPercentage = uniqueness(newPath, mainPath);
        cout << uniquenessPercentage << endl;

        /**
         * Create a variable to store the percenatge level that we need
         * to exceed to count the path that found as our alternate path
         */
        const double percentage = 0.2;

        if(uniquenessPercentage >= percentage) {
            double pathCost = 0;
            for (int i = 1; i < newPath.size(); i++) {
                theHeuristic = graph.crowFlyDistanceBetween(currentNode, end) / graph.maxRoadSpeed();
                pathCost += graph.edgeBetween(newPath[i - 1], newPath[i])->cost();
            }
           alternatePaths.enqueue(newPath, pathCost + theHeuristic);
        }
    }
    return alternatePaths.dequeue();
}

