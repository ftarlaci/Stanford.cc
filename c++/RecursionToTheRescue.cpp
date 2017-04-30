#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "set.h"
#include "vector.h"
#include "stdlib.h"
#include "strlib.h"
#include <cctype>
#include <string>
#include "sparsegrid.h"

using namespace std;

// function prototypes

bool scheduling(const Vector<Doctor>& doctors, const Vector<Patient>& patients,  Map<string, Set<string>>& schedule, int patientIndex);
bool disasterReady(const Map<string, Set<string>>& roadNetwork,  int numCities,  Set<string>& locations,  Set<string> uncovered);
bool dnaRecurse(const string& one, const string& two, int maxDistance, int editDistance, int indexStrandOne, int indexStrandTwo);
MinInfo winningGame(const Vector<State>& states, int requiredElectoralVotes, int stateIndex, SparseGrid<MinInfo> &memoizedResults);

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */

/** schedulingResult() is a helper function where I implement the recursive backtracking
 * algorithm to assign doctors to patients based on the availibity of the former and
 * the need of the latter. Every time the function recurses, it updates the patientIndex variable
 * that list of all the covered patients. */

bool schedulingResult(Vector<Doctor>& doctors,
                      Vector<Patient>& patients,
                      Map<string, Set<string>>& schedule, int patientIndex) {


    /** This is out base case where I am checking if there are any patient left in
the patient index. If there is none, that means schedulingResult has been able to every patient.
*/
    if(patientIndex == patients.size()) {
        return true;
    } else {
        for(int i = 0; i < doctors.size(); i++){
            if(doctors[i].hoursFree >= patients[patientIndex].hoursNeeded) {
                doctors[i].hoursFree -= patients[patientIndex].hoursNeeded;
                if(schedulingResult(doctors, patients, schedule, patientIndex + 1)) {
                    schedule[doctors[i].name].add(patients[patientIndex].name);
                    return true;
                }
                /** If the scheduling above does not cover all the patients, I need
                 * to undo what I changed above and put the number of hours back so
                 * that next time the function loops for another patient it can use those
                 * hours that did not work for the previous patient. */

                doctors[i].hoursFree += patients[patientIndex].hoursNeeded;
            }
        }
    }
    /** If the above code does not cover all the patients, I see that there
     * is no solution to this problem ( hours needed is more than hours available)
     *  and return false. */
    return false;
}

/** The main function we are given calls out helper function */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {


    /** Create copies of the vectors  in the original function we are given
     * which are passed as consts that are immutable so that
     * I can update them during our recursive calls. */

    Vector<Doctor> doctorsCopy = doctors;
    Vector<Patient> patientsCopy = patients;
    return schedulingResult(doctorsCopy, patientsCopy, schedule, 0);

}


/* * * * Disaster Planning * * * */

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */


/** Before I do anything with canBeMadeDisasterReady() function, I create
 * a helper function to compute disaster ready cities. The base case will be where
 * I am able to cover all the cities. "uncovered" represents the set of cities that
 * are currently uncovered. In the recursive step, I will create a variable for a supply
 * city and add it to the network. And then, I will check by covering the cit itself
 * as well as other cities adjacent to it, all the cities in the network are covered. If it
 * does not work meaning there are still cities that are not covered, out function will
 * return true. if not, I will undo our arrangment and return false to go back to the
 * recursive step again and trying a different supply center. */

bool disasterReady(const Map<string, Set<string>>& roadNetwork,
                   int numCities,
                   Set<string>& locations,
                   Set<string> uncovered) {

    if(uncovered.size() == 0){
        return true;
    }
    if(numCities == 0) {
        return false;
    }
    else {
        string uncoveredCity =  uncovered.first();
        for(string supplyCity : roadNetwork[uncoveredCity] + uncoveredCity) {
            locations.add(supplyCity);
            if(disasterReady(roadNetwork, numCities - 1, locations,
                             uncovered - roadNetwork[supplyCity] - supplyCity)){
                return true;
            }
            locations.remove(supplyCity);
        }
        return false;
    }
}

/** In our wrapper function, I will return our recursive helper function and
 * populate the set of uncovered cities  */

bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {

    Set<string> uncovered;
    for(string city : roadNetwork.keys()){
        uncovered += city;
    }
    return disasterReady(roadNetwork, numCities, locations, uncovered);
}



/* * * * DNA Detective * * * */

/**
 * Given two DNA strands and a maximum edit distance, returns whether those two strands are
 * within that edit distance of one another.
 *
 * @param one The first DNA strand.
 * @param two The second DNA strand.
 * @param maxDistance The maximum distance between them.
 * @return Whether the two strands are within that edit distance of one another.
 */

/** dnaRecurse is the helper function where I implement recursion to check both strands
 * by index number. For the base case, if the current index is the end of the strands
 * that means there is no characters left in the strand and same goes for the second
 * strand. If the edit distance is less than max distance the function returns. In the recursive
 * step, I check if the characters match. If they do, there is no edit distance. I move to the
 * next index position and if I need to make a change in the first strand, i add 1 to the edit
 * distance. If i need to make a change in the second strand I moev to the next if statement
 * and add an edit distance as well as move to the next idnex position in the second strand.
 * Next, if there is a need for insertion, i do that and update the edit distance accrodingly.
 * Same goes for the deletion.  */

bool dnaRecurse(const string& one, const string& two, int maxDistance, int editDistance, int indexStrandOne, int indexStrandTwo){

    if(editDistance > maxDistance) {
        return false;
    }
    if(indexStrandOne == one.size()) {   /** end of string one */
        two.size() - indexStrandTwo;
        return true;
    }

    if(indexStrandTwo == two.size()) {   /** end of string two */
        one.size() - indexStrandOne;
        return true;
    }

   /** If the characters match, there is no edit distance added and we move to the next strand*/
    if(one[indexStrandOne] == two[indexStrandTwo]) {
        if(dnaRecurse(one, two, maxDistance, editDistance, indexStrandOne + 1, indexStrandTwo + 1)) {
            return true;
        }
    }
    /** if we need to make a change in the strand one, we add it to the edit distance. */
    if(dnaRecurse(one, two, maxDistance, editDistance + 1, indexStrandOne +1 , indexStrandTwo + 1)) {
        return true;
    }

    /** If we get a match by inserting a character we add to the edit distance and the indexStrandTwo. */
    if(dnaRecurse(one, two, maxDistance, editDistance + 1, indexStrandOne, indexStrandTwo + 1)) {
        return true;
    }
    /** Finally the only other possibility we have is to delete a strand to reach at a match. */
    if(dnaRecurse(one, two, maxDistance, editDistance + 1, indexStrandOne + 1, indexStrandTwo)) {
        return true;
    }
    return false;
}

///** Wrapper function calls the recursive function.  */
bool approximatelyMatch(const string& one, const string& two, int maxDistance) {

    int indexStrandOne = 0;
    int indexStrandTwo = 0;
    int editDistance = 0;

    return dnaRecurse(one, two, maxDistance, editDistance, indexStrandOne, indexStrandTwo);
}

/* * * * Winning the Election * * * */

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 *
 *
 * To solve this problem, we have created a helper function where we implement our recursion
 * and memoization. In the helper function, we introduce two int variables to keep track
 * of the number of electoral votes we need and the states we have tried to find the
 * best combination, and the memoization grid. For our base case, we will ask if we need
 * any more electoral votes, if not, that means we have won. Additionally, we will also see if the
 * number of states we have to win equals to the number of states, in which case we return INT_MAX,
 * meaning there is nothing left we can do to win the election. If none of these bases cases apply, we go
 * into our recursive step and test what combination of states would give us the minimum number
 * of popular votes we need to win the election. With the help of memoization, we will be able to
 * store the combinations we have iterated and calculated so that we do not need to calculate the
 * subsets of states that we have already calculated. This will allow us to return results significantly
 * faster when we test the function on full elections data. As you will see below, we test each
 * combination by cehcking what happens if we include a given state or if we don't include it. The
 * best combination will give us the combination where the number of popular votes we need is lower.
 */

MinInfo winningGame(const Vector<State>& states, int requiredElectoralVotes, int stateIndex, SparseGrid<MinInfo>& memoizedResults){
    if(requiredElectoralVotes <= 0){
        return {0, {} };
    }
    if(stateIndex == states.size()){
        return {INT_MAX, {} };
    }
    //memoization
    if(memoizedResults.isSet(requiredElectoralVotes, stateIndex)) {
        return memoizedResults[requiredElectoralVotes][stateIndex];
    } else {
        /** calculate MinInfo with state at stateIndex */
        MinInfo stateIncluded = winningGame(states, requiredElectoralVotes - states[stateIndex].electoralVotes, stateIndex + 1, memoizedResults);

        /** calculate MinInfo without state at stateIndex */
        MinInfo stateExcluded = winningGame(states, requiredElectoralVotes, stateIndex + 1, memoizedResults);

        /** if/else to see which is "better", return with that one. Better will mean that
         * popularVotesNeeded is lower with the current combination of states to carry.
        */
        State currentState = states[stateIndex];
        int popularVotesToWinCurrentState = currentState.popularVotes / 2 + 1;

        if(stateIncluded.popularVotesNeeded != INT_MAX && stateIncluded.popularVotesNeeded + popularVotesToWinCurrentState < stateExcluded.popularVotesNeeded){
            stateIncluded.statesUsed.add(currentState);
            stateIncluded.popularVotesNeeded += popularVotesToWinCurrentState;
            memoizedResults[requiredElectoralVotes][stateIndex] = stateIncluded;
        } else {
            memoizedResults[requiredElectoralVotes][stateIndex] = stateExcluded;;
        }
        return memoizedResults[requiredElectoralVotes][stateIndex];
    }
}

/** In our wrapper function, we are calculating the total number
 * of electoral votes we need to win the election as well as calling
 * the helper function we wrote to return the combination of the states
 * to carry to win the election. */

MinInfo minPopularVoteToWin(const Vector<State>& states) {

    int sumOfElectoral = 0;
    for(State state : states){
        sumOfElectoral += state.electoralVotes;
    }

    /** need more than half of the electoral votes to win */
    int electoralVotesToWin = sumOfElectoral / 2 + 1;
    SparseGrid<MinInfo> memoizedResults(electoralVotesToWin + 1, states.size());
    return winningGame(states, electoralVotesToWin, 0, memoizedResults);
}
