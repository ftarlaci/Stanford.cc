#include <iostream>
#include "SimpleGraph.h"
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;


void Welcome();
void userInput();
const double kPi = 3.14159265358979323;

// Main method
int main() {

    Welcome();

    Edge * edgeViz = new Edge[200];
    Node * nodeViz = new Node[200];
    SimpleGraph graphViz;
    double * x0 = new double[200];
    double * y0 = new double[200];
    int nodeCount;



    InitGraphVisualizer(graphViz);
    userInput();

    // iterate over the graph nodes and append it to the nodes vector.
    for(int i = 0; i < nodeCount; ++i) {
        graphViz.nodes.push_back(nodeViz[i]);
    }

    // 3) place each node into its initial position using cmath functions cos and sin
    for(int j = 0; j < nodeCount; ++j) {
        graphViz.nodes[j].x = cos(3 * kPi * (j * 1));
        graphViz.nodes[j].y = sin(3 * kPi * (j * 1));
    }

    // 4) while the specified number of seconds has not elapsed, compute
    //the net forces on each node, move each node by the specified amount,
    // display the current state of the graph using the provided library.

    // invokes the time function to get the start time
    time_t startTime = time(NULL);

    // computes the difference in time between now and the start time.
    double elapsedTime = difftime(time(NULL), startTime);



// set k_repel and k_attract to 10 to the power of minus 3
    double k_repel = 0.001;
    double k_attract = 0.001;


    for(int i = 0; i < nodeCount; ++i) {
        x0[i] = 0.0;
        x0[i] = 0.0;

    }

// implement the force-directed layout algorithm. atan2 computes arc tangent with two
  // parameters. Compute repulsive forces for nodes.
    // http://www.cplusplus.com/reference/cmath/

   // not sure:  should I set while(true) as described in the assignment handout or
    //compute the remaining time here ???

    while(difftime(time(NULL), startTime) < elapsedTime) {
        for(int i = 0; i < nodeCount; ++i) {
            for(int j = 0; j < nodeCount; ++j) {

                // calculate repulsive forces
                double F_repel = k_repel / sqrt((graphViz.nodes[i].x - graphViz.nodes[j].x) *
                                                (graphViz.nodes[i].x - graphViz.nodes[j].x) +
                                                (graphViz.nodes[i].y - graphViz.nodes[j].y) *
                                                (graphViz.nodes[i].y - graphViz.nodes[j].y));



                x0[j] += F_repel * cos(atan2((graphViz.nodes[j].y - graphViz.nodes[i].y),
                                             (graphViz.nodes[j].x - graphViz.nodes[i].x)));

                y0[j] += F_repel * sin(atan2((graphViz.nodes[j].y - graphViz.nodes[i].y),
                                             (graphViz.nodes[j].x - graphViz.nodes[i].x)));

                y0[i] -= F_repel * sin(atan2((graphViz.nodes[j].y - graphViz.nodes[i].y),
                                            (graphViz.nodes[j].x - graphViz.nodes[i].x)));

                x0[i] -= F_repel * cos(atan2((graphViz.nodes[j].y - graphViz.nodes[i].y),
                                            (graphViz.nodes[j].x - graphViz.nodes[i].x)));

            }
        }

        // equivalent implementation for the edges.
        // how can I get the egdes struct start/ end?

        int i = 0;
        //size_t start =  graphViz.edges[i].start;
        // size_t end = graphViz.edges[i].end;
        double F_attract = k_attract;
        std::vector<Edge> edges;
        char filename;
        std::ifstream userinput(filename);



        // read from file: tutorialspoint.com/cplusplus/cpp_files_streams.htm
        // compute the net forces on each node with k_attract that controls
        // the the strength of attractive forces.

        // it not recognize the "filename" without declaration above.
        // helper function's scope is ignored here.

        // compute attractive forces

        while(filename>>edges[i].start>>edges[i].end) {
          F_attract = k_attract * ((graphViz.nodes[edges[i].start].x) -
                                    (graphViz.nodes[edges[i].start].x) *
                                    (graphViz.nodes[edges[i].start].x) -
                                    (graphViz.nodes[edges[i].start].x) +
                                    (graphViz.nodes[edges[i].start].y) -
                                    (graphViz.nodes[edges[i].start].y) *
                                    (graphViz.nodes[edges[i].start].y) -
                                    (graphViz.nodes[edges[i].start].y));


          // similar to the algorithm for nodes, get atan2

          x0[edges[i].start] += F_attract * cos(atan2((graphViz.nodes[edges[i].end].y -
                                                     graphViz.nodes[edges[i].start].y),
                                                     (graphViz.nodes[edges[i].end].x -
                                                      graphViz.nodes[edges[i].start].x)));

          y0[edges[i].start] += F_attract * sin(atan2((graphViz.nodes[edges[i].end].y -
                                                      graphViz.nodes[edges[i].start].y),
                                                      (graphViz.nodes[edges[i].end].x -
                                                       graphViz.nodes[edges[i].start].x)));


         x0[edges[i].end] -= F_attract * cos(atan2((graphViz.nodes[edges[i].end].y -
                                                   graphViz.nodes[edges[i].start].y),
                                                   (graphViz.nodes[edges[i].end].x -
                                                    graphViz.nodes[edges[i].start].x)));

         y0[edges[i].end] -= F_attract * sin(atan2((graphViz.nodes[edges[i].end].y -
                                                   graphViz.nodes[edges[i].start].y),
                                                   (graphViz.nodes[edges[i].end].x -
                                                    graphViz.nodes[edges[i].start].x)));

        }

        //append edges to the vector.

        for(int j = 0; j < i; ++j) {
            graphViz.edges.push_back(edges[j]);
        }

        // call Drawgraph to draw the graph we created after every iteration .
        DrawGraph(graphViz);

    }

    return 0;
}





/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

// 1. helper function to prompt the user for filename and time
void userInput() {
    char filename[50];
    cout << "Please enter the name of a graph file" << endl;
    std::ifstream userinput(filename);
    std::getline(std::cin,filename);

    // prompt user if the filename does not exist.
    if(userinput.fail()) {
        std::cerr << "Please enter a valid file name!" << endl;
    }
    // 3. prompt for time
    cout << "Enter a positive number in seconds to run the graphs" << endl;
}
