#include "iostream"
#include "stdio.h"

#include "stdlib.h"
#include "time.h"
#include "string.h"

#include "part1.h"
#include "part2.h"
#include "part3.hh"
#include "part4.hh"

using namespace std;

/* Main for the last (4th) part of the COMS327S20 Project. Reads a game
 * configuration from a file or generates a random one from the seed passed
 * as a command line argument. Afterwards, it displays the game on the terminal
 * with the help of ncurses library. It is supposed to provide a smooth interaction
 * with the user. If -f fileName is passed, the program reads from a file. If -s seed
 * is provided, the program generates a random game from the given seed. If none of
 * them are provided, a random game based on a random seed is generated. The user can
 * control the number of turns in case of -s seed switch with -1 or -3 (1 turns or 3 turns
 * at a time, respectively). He/She can also control the limit of the game with -l limit
 * switch (limit < 10), 'unlimited' if omitted.
 * @param argc: number of the command line arguments
 * @param argv: array of strings that represent command line arguments
 * @return 0 if the program is successfully executed
 * */
int main(int argc, char* argv[]){
  FILE *inFile = 0;
  char *fileName = 0;
  bool isRandom = false;
  unsigned rSeed = 0;
  
  int turns = 1, limit = 10, lcount = 0;
  
  for(int i=1; i<argc; i++){
    if(!strcmp(argv[i], "-f")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting an input file name after -f switch!\n");
      inFile = fopen(argv[++i], "r");
      fileName = argv[i];
    }
    else if(!strcmp(argv[i], "-s")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting a seed after -s switch!\n");
      rSeed = (int)atol(argv[++i]);
      isRandom = true;
    }
    else if(!strcmp(argv[i], "-1") || !strcmp(argv[i], "-3"))
      turns = argv[i][1] - '0';
    else if(!strcmp(argv[i], "-l")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting a limit after -l switch!\n");
      limit = (int)atol(argv[++i]);
    }
  }
  
  Card tableau[COLUMN][MAX_CARD], waste[MAX_STOCK], stock[MAX_STOCK], found[MAX_FOUND][MAX_CARD];
  int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
  
  if(!inFile && !isRandom){
    isRandom = true;
    srand((unsigned)time(NULL));
    rSeed = rand();
  }
  
  gamestate G;
  
  if(inFile){
    if(!part1(inFile, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit, &lcount))
      return 0;
    G = gamestate(tableau, waste, stock, found, tSize, cSize, wasteSize, stockSize, fSize, turns, limit);
  }
  else{
    G = generate_from_seed(rSeed, turns, limit);
  }
  
  try{
    part4(G, rSeed, turns, limit, fileName);
  }
  catch(const char* e){
    fprintf(stderr, "Error occured: %s\n", e);
  }
  
  return 0;
}
