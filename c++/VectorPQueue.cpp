/*************************************************************
 * File: VectorPQueue.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "VectorPQueue.h"
#include "error.h"
using namespace std;

VectorPriorityQueue::VectorPriorityQueue() {
 //vectorPQ = new VectorPriorityQueue();
}

VectorPriorityQueue::~VectorPriorityQueue() {
    while(!isEmpty()){
        dequeueMin();
    }
}

int VectorPriorityQueue::size() const {
    return vectorPQ.size();
}

bool VectorPriorityQueue::isEmpty() const {
    if(vectorPQ.size() == 0){
    return true;
    }
    return false;
}

void VectorPriorityQueue::enqueue(const string& value) {
  return;
}

string VectorPriorityQueue::peek() const {
    if(isEmpty()){
        error("The Queue is empty!");
    }
    return 0;
}

string VectorPriorityQueue::dequeueMin() {
    return 0;
}

