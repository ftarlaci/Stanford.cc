/*
 * File: RecursionProblems.cpp
 * --------------------------
 * Name: Fatma Tarlaci, Alex Weitzman (worked in pair)
 * Section: Jacob Wolf
 *
 * This program consists of different functions that solve different
 * recursion and graphical recursion questions.
 * Additionally, we take advantage of memoization and demonstrate
 * its power especially when used in a recursive function.
 */

// Please feel free to add any other #includes you need!
#include "Recursion.h"
#include <iostream>
#include <string>
#include "math.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gtypes.h"
#include "gwindow.h"
#include "stdlib.h"
#include "gobjects.h"
#include "graph.h"
#include "tokenscanner.h"
#include "console.h"
#include "simpio.h"
#include "gtable.h"



using namespace std;
double weightsMemoized(int row, int col, Vector<Grid<double> > &rowColumnMemoized);
const int PERSON_WEIGHT = 200;

/**
 * Given a street number and an avenue number, returns the number of
 * paths Karel has from that location back to the origin at 1st Street
 * and 1st Avenue, assuming Karel only moves left and down.
 * <p>
 * Notice that the coordinates here are 1-indexed.
 *
 * @param street Karel's starting street number.
 * @param avenue Karel's starting avenue number.
 * @return The number of paths back to (1, 1) moving only left and down.
 */
int numPathsHome(int street, int avenue) {
    // [TODO: delete the lines below this one and fill this function in. ]
    (void) street;
    (void) avenue;

    return 0;
}

/**
 * Given two strings, returns whether the second is a subsequence of
 * the first.
 *
 * @param text The larger text.
 * @param subseq The subsequence to search for.
 * @return Whether text has subseq as a subsequence.
 */
bool hasSubsequence(const string& text, const string& subseq) {
    // [TODO: delete the lines below this one and fill this function in. ]
    (void) text;
    (void) subseq;

    return false;
}

/**
 * Draws a Sierpinski triangle of the specified size and order, placing
 * its lower-left corner at position (x, y).
 *
 * @param window The window in which to draw the Sierpinski triangle.
 * @param x The x coordinate of the lower-left corner of the triangle.
 * @param y The y coordinate of the lower-right corner of the triangle.
 * @param sideLength The length of one side of the triangle.
 * @param order The order of the fractal.
 */
GPoint drawSierpinskiTriangleRec(GWindow& window, double x, double y, double sideLength, int order) {
    // Draws single triangle
    GPoint first = window.drawPolarLine(x, y, sideLength, 60);
    window.drawPolarLine(x, y, sideLength, 0);
    window.drawPolarLine(first.getX(), first.getY(), sideLength, -60);
    if (order > 0) {
        GPoint top = drawSierpinskiTriangleRec(window, x, y, sideLength / 2, order - 1);
        drawSierpinskiTriangle(window, top.getX(), top.getY(), sideLength / 2, order - 1);
        drawSierpinskiTriangle(window, x + sideLength/2, y, sideLength / 2, order - 1);
    }
    return first;
}

/**
 * Draws a Sierpinski triangle of the specified size and order, placing
 * its lower-left corner at position (x, y).
 *
 * @param window The window in which to draw the Sierpinski triangle.
 * @param x The x coordinate of the lower-left corner of the triangle.
 * @param y The y coordinate of the lower-right corner of the triangle.
 * @param sideLength The length of one side of the triangle.
 * @param order The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& window,
                            double x, double y,
                            double sideLength, int order) {
    drawSierpinskiTriangleRec(window, x, y, sideLength, order);
}
/**
 * Given a human pyramid in which each person weighs exactly 200 pounds,
 * returns the weight on the back of the person at the specified row and
 * column in the human pyramid. Rows and columns are zero-indexed, so,
 * for example, weightOnBackOf(2, 0) would give the weight on the back of
 * the leftmost person in the third row of the pyramid.
 *
 * @param row The zero-indexed row of the person in the pyramid.
 * @param col The zero-indexed column of the person in the pyramid.
 * @return The weight on the back of that person.
 */

/** This function uses memoization to store each computed row and col
 * weight for rows and cols already entered by the user and
 * will be used by weightonBackOf() to check if a certain row, col
 * has already been computed. If so, it returns the value to
 * weightOnBackOf().*/

double weightsMemoized(int row, int col, Grid<double>& rowColumnMemoized) {
    if(row == 0 && col == 0) {
        return 0;
    }
    if(col < 0 || row < 0 || col > row) {
        return 0;
    } else if(rowColumnMemoized.get(row, col) > 0) {
        return rowColumnMemoized.get(row, col);
    } else {
        double weightOnLeft = weightsMemoized(row-1, col-1, rowColumnMemoized);
        double weightOnRight = weightsMemoized(row-1, col, rowColumnMemoized);
        double personWeight;
        if (weightOnLeft == 0 || weightOnRight == 0) {
            personWeight = PERSON_WEIGHT;
        } else {
            personWeight = PERSON_WEIGHT * 2;
        }
        rowColumnMemoized[row][col] = (weightOnLeft + weightOnRight + personWeight) / 2.0;
        return (weightOnLeft + weightOnRight + personWeight) / 2.0;
    }
}

/** wrapper function that calls memoized results function. */

double weightOnBackOf(int row, int col) {
    Grid<double> rowColumnMemoized(row + 1, col + 1);
    rowColumnMemoized.fill(0.0);
    return weightsMemoized(row, col, rowColumnMemoized);
}


/**
 * Helper function that, given two drill sites, returns the distance
 * between them. We've provided this function for you; you don't need
 * to modify it.
 *
 * @param a The first drill site.
 * @param b The second drill site.
 * @return The distance between them.
 */


double drillDistance(const DrillSite& a, const DrillSite& b) {
    return sqrt(pow(a.pt.getX() - b.pt.getX(), 2.0) + pow(a.pt.getY() - b.pt.getY(), 2.0));
}

/**
 * Helper function that, given a list of drill sites in order, returns
 * the total length the drill would take to drill in those sites in that
 * order and then return to the starting point.
 *
 * @param path The list of drill sites to visit, in order.
 * @return The cost of drilling those sites and returning to the starting point.
 */


double drillRouteLength(const Vector<DrillSite>& path) {
    double length = 0;
    for (int i = 0; i < path.size(); i++) {
        length += drillDistance(path[i], path[(i + 1) % path.size()]); // !
    }
    return length;
}

/**
 * Given a list of drill sites, returns the best order in which to
 * drill all of those sites, assuming that the drill returns to the
 * original starting drill site after visiting the others.
 *
 * @param sites The sites that need to be drilled.
 * @return The best order in which to visit them.
 */



/** Given a list of sites, this helper function permutes all the paths
 * and then returns the shortest path among them for the best drill route. */


void permuteAllPaths(Vector<DrillSite>& sitesLeft, Vector<DrillSite>& currentPath, Vector<DrillSite>& shortestPath) {
    if(sitesLeft.isEmpty()) {
        if(shortestPath.isEmpty() || drillRouteLength(currentPath) < drillRouteLength(shortestPath)) {
            shortestPath = currentPath;
        }
    } else {
        int numSites = sitesLeft.size();
        for(int i = 0; i < numSites; i++) {
            DrillSite site = sitesLeft[i];
            sitesLeft.remove(i);
            currentPath.add(site);
            permuteAllPaths(sitesLeft, currentPath, shortestPath); // use the drill
            currentPath.remove(currentPath.size() - 1);
            sitesLeft.insert(i, site);
      }
    }
}

Vector<DrillSite> bestDrillRouteFor(const Vector<DrillSite>& sites) {
    Vector<DrillSite> currentPath;
    Vector<DrillSite> shortestPath;
    Vector<DrillSite> sitesLeft = sites;
    permuteAllPaths(sitesLeft, currentPath, shortestPath);
    return shortestPath;
}

/**
 * Given a list of candidate hospital sites and an amount of available funds,
 * returns the list of hospitals to be built that provides coverage to the
 * greatest number of cities.
 *
 * @param sites The list of possible hospitals to build.
 * @param fundsAvailable The amount of money you have available.
 * @return The best choice of hospitals to build given available funds.
 */
Vector<Hospital> bestCoverageFor(const Vector<Hospital>& sites,
                                 int fundsAvailable) {
    // [TODO: delete the lines below this one and fill this function in. ]
    (void) sites;
    (void) fundsAvailable;

    return {};
}


