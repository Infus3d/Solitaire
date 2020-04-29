#ifndef PART4_HH
#define PART4_HH
  
  #include "part1.h"
  #include "part2.h"
  #include "part3.hh"
  
  /* Generates a random game from the given seed rSeed. Turns and limit of
 * the game is not determined by the randomness and instead are provided
 * as an argument. The logic of the randoming below is simple: shuffle the
 * initially oredered deck of 52 cards and assign them to wherever we want
 * starting from the beginning. The function always returns the tableau
 * with 1 open card in every column and (i-1) covered cards in every ith column
 * (1 <= i <= 7). The rest are kept in stock.
 * @param rSeed: the given seed to decide the randomness upon
 * @param turns: given number of turned cards at a time
 * @param limit: given limit of the resets in the game( < 10), unlimited if 10.
 * @return randomly generated gamestate from the given rSeed
 * */
  gamestate generate_from_seed(unsigned rSeed, int turns, int limit);
  
  /* This is the 4th part of the COMS327S20 Project. Takes in an initial game state
 * whether it is randomly generated or read from a file, then displays the game
 * on the terminal emulator with the help of ncurses library. The game has multiple
 * options like Quit, Restart, New Game, Save to file, Undo and of course the game's
 * moves. The screen is split into 3 main(table, misc, hotkeys) and 1 secondary (status) windows.
 * Pressing "New Game" initiates a new random game with seed chosen at random too. The source
 * of the game (file or seed) is displayed to the user. Owing to the status window,
 * the user might have smoother interaction with the game.
 * Maximum of 1000 consequtive moves is allowed, excluding the ones that were un-done.
 * @param H: given initial gamestate
 * @param _rSeed: the seed that the randomness of the initial game state is based on
 * @param _turns: the number of cards to turn at a time
 * @param _limit: maximum number of allowed resets
 * @param fileName: the name of the file that the game state was read from, if applicable
 * */
  void part4(gamestate H, unsigned rSeed, int turns, int limit, char* fileName);

#endif
