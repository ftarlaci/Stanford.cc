/*
 * File: WordLadders.cpp
 * --------------------------
 * Name: Fatma Tarlaci
 * Section: Jacob Wolf
 * [TODO: rewrite the documentation] ??
 */

#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include <iterator>
#include "set.h"
#include "queue.h"
#include "simpio.h"
#include <vector>
#include "RecordLadders.h"

using namespace std;

/** Create a vector for the final ladder with the final word */
Vector<string> getLadder (string startWord, string finalWord, Lexicon& lexicon, Set<string>& mySet);

/** Create a Set to store the words already enqueued.*/
Set<string> mySet;

/** Create an empty queue of type vector to store ladders */
Queue<Vector<string>> ladderQueue;

/** Create a Lexicon */
// Lexicon lexicon("EnglishWords.dat");


int main() {

    /** Get words from user*/
    string startWord = "";
    string finalWord = "";

    /** Create a Lexicon */
    Lexicon lexicon("EnglishWords.dat");

    bool keepPlaying = true;
    while(keepPlaying) {
        cout << "Give me two English words, and I will change the first into the second by replacing one letter at a time." << endl;
        startWord = getLine();
        finalWord = getLine();
        /** Check user input to see if they are contained in the lexicon and if they are equal length */
        while (!(startWord.length() == finalWord.length() && lexicon.contains(finalWord) && lexicon.contains(finalWord))) {
            cout<< "The words you entered have different length. Please enter words of the same length." << endl;
            startWord = getLine();
            finalWord = getLine();
        }

        mySet.add(startWord);
        Vector<string> finalLadder = getLadder(startWord, finalWord, lexicon, mySet);
        // check if ladder is valid, if it is, recordLadderBetween() + print the ladder, else, recordNoLadderBetween()

        /** if a ladder is found */
        if(!finalLadder.isEmpty()) {
            recordLadderBetween(startWord, finalWord, finalLadder);
            cout << "Found the shortest ladder! Here it is: " << finalLadder << endl;
        } else {
            cout << "There is no ladder between your words!" << endl;
            recordNoLadderBetween(startWord, finalWord);
        }

        /** Prompt the user if they want to play again.*/
        keepPlaying = getYesOrNo("Want to find another word ladder? (y/n))");



    }
    return 0;
}

Vector<string> getLadder(string startWord, string finalWord, Lexicon& lexicon, Set<string>& mySet) {
    Vector<string> firstLadder;
    firstLadder.add(startWord);

    /** Add the start word to the end of the queue */
    ladderQueue.enqueue(firstLadder);

    while(!ladderQueue.isEmpty()) {

        /** Create a ladder variable to return if a ladder is found */
        Vector<string> nextLadder = ladderQueue.dequeue();
        string lastWord = nextLadder.get(nextLadder.size()-1);

        /** check if the final word in this ladder is the destination word*/
        if(lastWord == finalWord) {
            return nextLadder;
        }

        /** iterate for each word in the lexicon the EnglishWords.dat that differs by one letter. */
        for (char letter = 'a'; letter <= 'z'; letter++) {
            for (int i=0; i < lastWord.length(); i++) {
                string newAlternative = lastWord.substr(0,i) + letter + lastWord.substr(i+1);
                if(lexicon.contains(newAlternative) && !mySet.contains(newAlternative)) {
                    mySet.add(newAlternative);

                    Vector<string> ladder3 = nextLadder;
                    ladder3.add(newAlternative);
                    ladderQueue.enqueue(ladder3);
                }
            }
        }
    }
    return Vector<string>();
}












