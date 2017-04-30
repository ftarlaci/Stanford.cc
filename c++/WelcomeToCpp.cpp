/*
 * File: WelcomeToCpp.cpp
 * --------------------------
 * Name: Fatma Tarlaci
 * Section: Jacob Wolf
 * This file is the starter project for the first assignment of CS106B.
 * Each of the functions below solves certain problems and can be used
 * for different reasons. Converting numbers to strings and string to
 * functions, counting how many flips it takes to get a certain number
 * of heads, compute Pascal's triangle on a number input, and finally
 * count the number of sentences, words, and syllables in a given file.
 *
 * The repeating call sequence is
 *    triggerStackOverflow(137), which calls
 *    triggerStackOverflow(549), which calls
 *    triggerStackOverflow(327) which calls
 *    triggerStackOverflow(203) which calls
 *    triggerStackOverflow(685) which calls
 *    triggerStackOverflow(164) which calls
 *    triggerStackOverflow(137) again.
 */

// Please feel free to add any other #includes you need!
#include "WelcomeToCpp.h"
#include <iostream>
#include <string>
#include "strlib.h"
#include "random.h"
#include <cstdlib>
#include <cctype>
#include <fstream>
#include "console.h"
#include "simpio.h"
#include <fstream>
#include "TokenScanner.h"
#include "strings.h"

using namespace std;

/**
 * flipHeads uses the type enum variables HEADS
 * and TAILS to count three consecutive heads
 * in flipping a coin. It uses the library function randomChance
 * to get 0.50 chance of flipping the head of a coin. When
 * three consecutive heads are hit,it prints the result and
 * counts how many times it took to reach that result.
*/

void flipHeads() {   
    int numHeads = 0;
    int flipsTotal = 0;

    while(numHeads < 3) {
        if(randomChance(0.50)) {
            cout << "Flip: heads" << endl;
            numHeads++;
        }else {
            cout << "Flip: tails" << endl;
            numHeads = 0;
        }
        flipsTotal++;
    }
    cout <<"It took " << flipsTotal << "to get 3 consecutive heads." << endl;
}

/**
 * nChoosek computes binomical coefficients
 * recursively by first checking if k equals to 0 or n.
 * This constitutes the base case. The recursive step
 * computes numbers that are bigger than zero by addition
 * and substraction of n and k and returns an int as a result.
*/

int nChooseK(int n, int k) {
    if(k == 0 || k == n) {
        return 1;
    }else {
        return nChooseK(n -1, k) + nChooseK(n-1, k-1);
    }
}


/** This function does just the opposite of stringToInt
 * function and converts an integer to a string. It
 * uses recursion to do this. For the base case, it checks
 * if the string is empty or not for the base case. If it is not,
 *  it moves to the recursive step and parses out the last
  character of the string and implements the computation. */

string intToString(int n) {
    if(n >= 0 && n < 10) {
        char ch = char(n + '0');
        string str(1, ch);
        return str;
    } else{
        return intToString(n / 10) + char((n % 10) + '0');

    }
    return 0;
}

/**
 * This function converts a string into its integer
 * equivalent recursively. For the base case, it
 * checks if the string is empty. If it is, it returns true
 * and does not compute the recursive step. If it the string
 * is not empty, it moves to the recursive step and converts the
 * string to integer.
*/

int stringToInt(string str) {
    if(str == "") {
        char ch = '0';
        int val = ch - '0';
        return val;
    }
    if(str[0] == '-'){
        int val = str[str.length() - 1] - '0';
        string subString = str.substr(1, str.length() - 1);
        return (stringToInt(subString)) * 10 + val;
    }
    int val = str[str.length() - 1] - '0';
    string subString = str.substr(0, str.length() - 1);
    return (stringToInt(subString)) * 10 + val;
}



/**
 * This is a helper function computes the number
 * of vowels in a given token by checking if the last
 *  letter read is a vowel. It also covers the case where
 * the end letter of a word is 'e' and does not include it
 * in the number of syllables that are counted. @param is token,
 * the word that is being read. @return the number of vowels
 * which will later be used to count number of syllables in a given file.
*/


int countSyllables(string token) {
    int num = 0;
    bool lastWasVowel = false;
    for(int i= 0; i < token.length(); i++){
        char ch = token[i];
        if(ch == 'a'|| ch == 'e' || ch == 'i' || ch =='o' || ch == 'u' || ch == 'y'){
            if(lastWasVowel == false) {
                if(!(ch == 'e' && (token.length() - 1) == i)){
                    num++;
                }
            }
            lastWasVowel = true;
        }else{
            lastWasVowel = false;
        }
    }
    if(num == 0) {
        num = 1;
    }
    return num;
}

/**
 * statisticsFor function reads a source file from istream and
 * counts number of syllables, words, and sentences in the given
 * file. The while loop will scan the source file and count number
 * of sentences, words, and uses the helper function to count syllables.
 * The first if statement checks number of sentences and add one if you
 * find any of these three punctions, denoting the end of a sentence,
 * exists and add to the number of sentences.The implementation
 * applied to the number of words and syllables, which additionally uses
 * a helper function.
*/


DocumentInfo statisticsFor(istream& source) {
    DocumentInfo docInfo = { 0, 0, 0 };

    TokenScanner scanner(source);
    scanner.ignoreWhitespace();
    scanner.addWordCharacters("'");

    while(scanner.hasMoreTokens()){
        string token = scanner.nextToken();
        token = toLowerCase(token);
        if(token == "." || token == "!" || token == "?") {
            docInfo.numSentences++;
        }

        if(isalpha(token[0])) {
            docInfo.numWords++;
            docInfo.numSyllables += countSyllables(token);
        }
    }

    if(docInfo.numSentences == 0) {
        docInfo.numSentences = 1;
    }
    if(docInfo.numWords == 0) {
        docInfo.numWords = 1;
    }
    return docInfo;
}
