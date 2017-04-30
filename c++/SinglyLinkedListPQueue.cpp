/*************************************************************
 * File: SinglyLinkedListPQueue.cpp
 *
 * Implementation file for the SinglyLinkedListPriorityQueue
 * class.
 *
 */
 
#include "SinglyLinkedListPQueue.h"
#include "error.h"
using namespace std;

SinglyLinkedListPriorityQueue::SinglyLinkedListPriorityQueue() {
head = tail = nullptr;
}

SinglyLinkedListPriorityQueue::~SinglyLinkedListPriorityQueue() {
    while(!isEmpty()){
        (void) dequeueMin();
    }
}

int SinglyLinkedListPriorityQueue::size() const {	
    return numElems;
}

bool SinglyLinkedListPriorityQueue::isEmpty() const {
    if(size() == 0){
		return true;
    }
    return false;
}

void SinglyLinkedListPriorityQueue::enqueue(const string& value) {
	// TODO: Fill this in!
    (void) value;
}

string SinglyLinkedListPriorityQueue::peek() const {
    return 0;
}

string SinglyLinkedListPriorityQueue::dequeueMin() {
    string result = peek();

    Cell * next = head->next;
    delete head;
    head = next;

    if(head == nullptr){
        tail = nullptr;
    }
    numElems--;
    return result;
}

