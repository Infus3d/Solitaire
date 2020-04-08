#ifndef PART2_H
#define PART2_H
  // Useful constant
  #define INF 1e6+7
  
  /* This is the second part of COMS327S20 Project. Takes in the given
   * state of the game until "MOVES:" section, and processes the section
   * until the end of file. If there is a formatting error, line number of
   * the illegal format is printed out. If the format is correct but the
   * input is invalid for the current state of the game, the move number
   * is printed out. If there is neither formatting error nor invalid input
   * then total number of processed moves is printed out.
   * @param inFile: given file buffer to read the data from
   * @param tableau: literal tableau of cards of the given state of the game
   * @param tSize: contains sizes of each columns of tableau
   * @param cSize: contains sizes of covered cards in each column of the tableau
   * @param waste: contains the cards that are in the waste currently. Last card of 
   *               the array is the topmost card in the waste
   * @param wasteSize: size of the waste array
   * @param stock: contains the cards that are in the stock currently. Last card of
   *               the array is the topmost card in the stock
   * @param stockSize: size of the stock array
   * @param found: contains cards that are in 4 foundations.
   * @param fSize: the size of the each foundation
   * @param turns: number of the cards that are turned over from stock to the waste
   * @param limit: max number of times the waste can be reset back to the stock. Usually
   *               is in between 0 and 9. Indicates infinity (unlimited) if 10.
   * @param lcount: current line number before processing MOVES section
   * @return 1 if the file has been successfully read, 0 if there is a formatting error
   *         or -1 if the provided move is not valid for that instant of the game state
   * */
  int part2(FILE* inFile, int maxMoves, struct Card tableau[][MAX_CARD], int tSize[COLUMN], int cSize[COLUMN], 
          struct Card waste[MAX_STOCK], int* wasteSize, struct Card stock[MAX_STOCK], int* stockSize, struct Card found[MAX_FOUND][MAX_CARD], 
          int fSize[MAX_FOUND], int* turns, int* limit, int* lcount);
#endif
