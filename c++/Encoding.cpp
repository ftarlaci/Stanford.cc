/**
 * Author: Fatma Tarlaci
 * This program implements Huffman Encoding algorithm to read an input file,
 * creates an encoding tree for its characters and then encodes and decodes
 * the bit patterns created for each character. Finally it compresses and d
 * ecompresses the input file.
*/


#include "Encoding.h"
#include "priorityqueue.h"
#include <iostream>
#include "filelib.h"
#include "map.h"


/** Builds a map of frequency for each character in the input file.
 * Keys are characters and values are the frequency number of that
 * character.
*/

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    char ch;
    while(input.get(ch)){
        int character = int (ch);  /** typecast the character */
        freqTable[character]++;

    }
    /** Adding end of the file character into our map. */
    freqTable[PSEUDO_EOF]++;
    return freqTable;
}


/** Builds the encoding tree where we will use a priority queue. In each
 * step we dequeue the smallest two elements from the queue and create a
 * mini tree where the root will be the sum of the frequency values of the
 * two original nodes. This sum node will be an "intermediate node" and its
 * children will have the frequency and the characters in their keys.
 * @param map of the frequency table
 * @return priority queue dequeue.
*/

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    /** create a priority queue */
    PriorityQueue<HuffmanNode *> priorityQ;

    /** place occurences in the priority queue */
    for(int character : freqTable) {

        /** creating  a new huffman node for the node we will put back on the PQ */
        HuffmanNode * tobeEnqueued = new HuffmanNode;
        tobeEnqueued->character = character;
        tobeEnqueued->count = freqTable[character];
        tobeEnqueued->zero = nullptr;
        tobeEnqueued->one = nullptr;

        priorityQ.enqueue(tobeEnqueued, tobeEnqueued->count);
    }

    /** while the priority queue has more than one element, we will keep dequeuing
 * the first two smallest element and create a tree with a single node, which will
 * put it back on the priority queue.
*/
    while(priorityQ.size() > 1) {

        /** dequeue the first two elements from the pq and build a new tree from those two values. */
        HuffmanNode * dequeuedFirst = priorityQ.dequeue();
        HuffmanNode * dequeuedSecond = priorityQ.dequeue();

        /** create the intermediate node, which will carry the sum of the frequencies of the two dequeued nodes */
        HuffmanNode * intermediateNode = new HuffmanNode;
        intermediateNode->character = NOT_A_CHAR;
        intermediateNode->count = dequeuedFirst->count + dequeuedSecond->count; /** intermediate is the sum */
        intermediateNode->zero = dequeuedFirst;
        intermediateNode->one = dequeuedSecond;

        /** put back the sum in the PQ. */
        priorityQ.enqueue(intermediateNode, intermediateNode->count);
    }
    return priorityQ.dequeue();
}


/** delete the children and root nodes of the tree to clean up the memory. */
void freeTree(HuffmanNode* node) {
    if (node == nullptr) return;
    freeTree(node->zero);
    freeTree(node->one);
    delete node;
}


/** getEncoing() is the recursive helper function to inorder traverse the tree and record
 * the bit pattern for each character at each leaf, creating a map.
 * @param encodingTree that we created to assign the bit patterns.
 * @param encodingMap where we assigned each character a bitPattern
 * @param bitPattern where we store the bit pattern for each character
*/
void getEncoding(HuffmanNode* encodingTree, Map<int, string>& encodingMap, string bitPattern){
    int character = encodingTree->character;
    if(encodingTree == nullptr){
        return;
    }

    /** if we are at a leaf, it means we now have a character that we need to store in the map */
    if(encodingTree->isLeaf()){
        encodingMap.add(character, bitPattern);
        return;
    } else {

/** if we are not at a leaf, we will create the bit pattern by adding 0
 * for the left child and 1 for the right child.
*/
        getEncoding(encodingTree->zero, encodingMap, bitPattern + "0");// changed the order here; it might've got broken
        getEncoding(encodingTree->one, encodingMap, bitPattern + "1");
    }
}

/** wrapper function for getEncoding() that creates the map of bitpatterns. */
Map<int, string> buildEncodingMap(HuffmanNode * encodingTree) {
    int character;
    string bitPattern = "";
    Map<int, string> encodingMap;
    getEncoding(encodingTree, encodingMap, "");
    return encodingMap;
}


/** encodeData function takes in the encodingMap we created and the input
 * file as its parameters to output the bit encoding of each character.
*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char ch;
    while(input.get(ch)){
        if(encodingMap.containsKey(ch)){
            string bits = encodingMap[ch];
            // if(ch == PSEUDO_EOF) break;
            for(char bit : bits){
                if(bit == '0'){
                    output.writeBit(0);
                }
                if(bit == '1'){
                    output.writeBit(1);
                }
            }
        } else {
            cout << "no character is found" << endl;
        }
    }
    /**
     * After finishing up rading all the characters from the input file, we need to
     * tell the encoding function to encode the end of file character. Just
     * like we wrote the bit pattern for each character, we will create the bit
     * pattern for PSEUDO_EOF.
    */
    for(char bit : encodingMap[PSEUDO_EOF]){
        if(bit == '0'){
            output.writeBit(0);
        }
        if(bit == '1'){
            output.writeBit(1);
        }
    }
}


/** decodeData function decodes the bits from the input file and
* writes the result to the output stream.
* @param input is the input file given to the program to decode
* @param encodingTree is a pointer to the root of our tree.
* @param output is how we output the decoded bitpattern for each character.
*/

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
/**
 * Create a new node to keep track of the current node, while traversing the tree.
*/
        HuffmanNode * currentNode = encodingTree;

/** The while loop traverses the tree checking if it is at leaf and if so,
 * it stores the given character.
*/
        while(true){
            if(currentNode->isLeaf()){
                if(currentNode->character == PSEUDO_EOF) break;
                output.put(currentNode->character);
                currentNode = encodingTree;
            } else {
                int bit = input.readBit();
                if(bit == 0) {
                    currentNode = currentNode->zero;
                }
                if(bit == 1) {
                    currentNode = currentNode->one;
                }
            }
        }
    }


/** Compress function uses the frequency table we created to read
 * the text file and to output a stream of bits.
 * @param input, @param output are the parameters that will take
 * in a stream and output a obitstream. Here we are simply calling
 * the parts of the Huffman encoding process that we have created,
 * which will put the entire Huffman Encoder and Decoder together.
*/
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode * encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);

    output << freqTable;
    /** Before we make a second pass of the characters, we should rewind. */
    rewindStream(input);
    encodeData(input, encodingMap, output);

}

 /** Using what we have encoded, decompress function takes
 * @param ibitstream as a text input and
 * @param ostream as a bit output.It writes the frequency
 * table to the output before it starts writing any bits.
*/
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;

    /** Populating the tree with the frequency table */
    HuffmanNode * encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);

    /** Finally, we clean our mess! */
    freeTree(encodingTree);
}

/** Works! Yay! */

