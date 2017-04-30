/*************************************************************
 * File: DoublyLinkedListPQueue.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */

#include "DoublyLinkedListPQueue.h"
#include "error.h"
using namespace std;


DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
    head = nullptr;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    while(!isEmpty()){
        dequeueMin();
    }
}

int DoublyLinkedListPriorityQueue::size() const {	
    return numElems;
}

bool DoublyLinkedListPriorityQueue::isEmpty() const {
    return numElems == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(const string& value) {
    Cell * newCell = new Cell;
    newCell->value = value;
    newCell->next =  head;
    newCell->prev = nullptr;

    if(head != nullptr) {
        head->prev = newCell;
    }
    head = newCell;
    numElems++;
}

string DoublyLinkedListPriorityQueue::peek() const {
    if (isEmpty()) {
        error("The priority queue is empty.");
    }
    Cell * smallest = head;
    Cell * currentSmallest = head;

    // iterate over the list to find the smallest and assign the currentSmallest to the smallest
    for(currentSmallest = head; currentSmallest != nullptr; currentSmallest = currentSmallest->next){
        if(currentSmallest->value < smallest->value ){
            smallest = currentSmallest;
        }
    }
    // store the value so we can return it
    string smallestString = smallest->value;
    return smallestString;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
    if(isEmpty()){
        error("The priority queue is empty");
    }

    Cell * smallest = head;
    Cell * currentSmallest = head;

    // iterate over the list to find the smallest and assign the currentSmallest to the smallest
    for(currentSmallest = head; currentSmallest != nullptr; currentSmallest = currentSmallest->next){
        if(currentSmallest->value < smallest->value ){
            smallest = currentSmallest;
        }
    }
    // store the value so we can return it
    string smallestString = smallest->value;

    if (smallest == nullptr){
        error("There is nothing in the queue.");
    }
    if (smallest->prev != nullptr && smallest->next != nullptr){
        // rewire the list
        smallest->prev->next = smallest->next;
        smallest->next->prev = smallest->prev;
    }
    else if (smallest->prev == nullptr && smallest->next == nullptr){
        delete smallest;
        head = nullptr;
        numElems--;
        return smallestString;
    }
    else if (smallest->prev == nullptr){
        smallest->next->prev = nullptr;
        head = smallest->next;
    }
    else {
//        smallest->next == nullptr;
        smallest->prev->next = nullptr;
    }
    delete smallest;
    numElems--;
    return smallestString;
}
