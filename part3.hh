#ifndef PART3_HH
#define PART3_HH
  #include "iostream"
  #include "string"
  #include "vector"
  
  using namespace std;
  
  /* A struct that defines a game state of a Klondike Solitaire game.
   * */
  struct gamestate{
    // Variables to keep track of the game mode: turn number and limit of resetting the waste back to the stock
    int turns, limit;
    // Arrays to keep track of the tableau, waste, stock and foundation respectively
    Card tableau[COLUMN][MAX_CARD], waste[MAX_STOCK], stock[MAX_STOCK], found[MAX_FOUND][MAX_CARD];
    // Arrays and variables to keep track of the size of the tableau columns, tableau seperators, waste, stock and foundation respectively
    int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
    
    /* Constructor for gamestate.
     * @param _tableau: cards in the tableau
     * @param _waste: cards in the waste
     * @param _stock: cards in the stock
     * @param _found: cards in the foundation
     * @param _tSize: column sizes of the tableau's 7 columns
     * @param _cSize: number of covered cards in each column of the tableau: position of '|' character
     * @param _wasteSize: size of the waste
     * @param _stockSize: size of the stock
     * @param _fSize: size of the 4 different suits of the foundation; number of cards in each of them
     * @param _turns: number of the cards to turn with '.' move
     * @param _limit: number of further the resets allowed; if 10, it is unlimited
     * */
    gamestate(Card _tableau[][MAX_CARD], Card _waste[MAX_STOCK], Card _stock[MAX_STOCK], Card _found[][MAX_CARD], 
              int _tSize[COLUMN], int _cSize[COLUMN], int _wasteSize, int _stockSize, int _fSize[MAX_FOUND], int _turns, int _limit);
    
    /* A function that returns a "string view" of the game state. It is in almost
     * the same exchange format that was parsed in part1 and part2, except for that
     * the foundation, rules and moves parts are omitted.
     * @returns the "string form" of the game state.
     * */
    string toString();
  };
  
  /* This the third part of the COMS327S20 Project. Takes in an initial game state and the maximum number
 * of allowed moves and returns true if therer is a sequence of moves that leads to a "winning" game 
 * configuration. The sequence is pushed into the vector that is provided as a parameter. 
 * If the cached parameter is enabled, the function employs a cache (hashtable) that helps to reduce 
 * the overall search time by keeping track of the game states.
 * If forced parameter is enabled, the search forces the "safe" moves defined by the isSafe function,
 * which may eventually lead to a faster search while might also increase the # of required to win.
 * If vformat is enabled, it reports to stderr the search progress during the search and the number 
 * of configurations checked, number of cache entries and the total time elapsed after the search.
 * @param G: given initial state of the game
 * @param ans: a vector that stores the sequence of moves leading to winnable state
 * @param maxS: the maximum allowed number of moves
 * @param cached: if true, the search employs a "cache"
 * @param forced: if true, the search forces "safe" moves
 * @param vformat: if true the search enters a verbose mode
 * @return true if there is a sequence of moves for winnable configuration under the given limit, false otherwise.
 * */
  bool part3(gamestate G, vector<string>& ans, int maxS, bool cached, bool forced, bool vformat);

#endif
