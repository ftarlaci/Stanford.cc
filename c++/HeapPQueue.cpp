/*************************************************************
 * File: HeapPQueue.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "HeapPQueue.h"
#include "error.h"
using namespace std;

HeapPriorityQueue::HeapPriorityQueue() {
    //heapQueue = new HeapPriorityQueue();
}

HeapPriorityQueue::~HeapPriorityQueue() {
    while(!isEmpty()){
        dequeueMin();
    }
}

int HeapPriorityQueue::size() const {	
    return numElems;
}

bool HeapPriorityQueue::isEmpty() const {
    if(numElems == 0){
        return true;
    }
    return false;
}

void HeapPriorityQueue::enqueue(const string& value) {
	// TODO: Fill this in!
    (void) value;
}

string HeapPriorityQueue::peek() const {
	// TODO: Fill this in!
	
	return "";
}

string HeapPriorityQueue::dequeueMin() {
//    while(!isEmpty()){

//    }
	
	return "";
}


