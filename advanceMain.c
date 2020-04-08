#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "part1.h"
#include "part2.h"

/* Simulation of a Klondike Solitaire with Part1 and Part2 (not finished yet).
 * Takes in any game configuration and determines if it has formatting error,
 * or invalid input for the current game state. If none of these happen,
 * then the program parses the input completely and outputs the final state
 * of the game. Note the game outputs the final configuration even if
 * invalid input were encountered, in which case it first prints out the
 * number of the invalid move and then the state of the game paused at that
 * instant. If -m N switch is provided, the program processes at most N
 * moves from the given initial MOVES: section; If -m N is ommitted this numer
 * is set to INFINITY or unlimited moves. If -x is provided, the program
 * outputs the final state in exchange format instead of human-readable format
 * when omitted. Outputs to a given file by -o fileName switch or stdout if omitted.
 * @param argc: number of the command line arguments
 * @param argv: array of strings that represent command line arguments
 * @return 0 if the program is successfully executed
 * */
int main(int argc, char* argv[]){
  FILE* inFile = stdin; FILE* outFile = stdout;
  char* infileName;
  int maxMoves = INF, exFormat = 0;
  for(int i=1; i<argc; i++){
    if(!strcmp(argv[i], "-m")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting a number after -m switch!\n");
      maxMoves = (int)atol(argv[++i]);
    }
    else if(!strcmp(argv[i], "-o")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting a filename after -o switch!\n");
      outFile = fopen(argv[++i], "w");
    }
    else if(!strcmp(argv[i], "-x"))
      exFormat = 1;
    else if(argv[i][0] != '-')
      inFile = fopen(argv[i], "r"), infileName = argv[i];
  }
  
  if(!inFile)
    return 0 * fprintf(stderr, "Input is invalid: File %s does not exist!\n", infileName);
  
  int turns, limit, lcount;
  struct Card tableau[COLUMN][MAX_CARD], waste[MAX_STOCK], stock[MAX_STOCK], found[MAX_FOUND][MAX_CARD];
  int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
  
  if(!part1(inFile, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit, &lcount))
    return 0;
  
  int ret = part2(inFile, maxMoves, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit, &lcount);
  if(!ret) return 0;
  if(exFormat){
    /************************ Exchange format ***************************/
    fprintf(outFile, "RULES:\nturn %d\n", turns);
    if(limit == 10) fprintf(outFile, "unlimited\n");
    else  fprintf(outFile, "limit %d\n", limit);
    fprintf(outFile, "FOUNDATIONS:\n");
    for(int i=0; i<MAX_FOUND; i++){
      fprintf(outFile, "%c%c", found[i][fSize[i]-1].rank, found[i][fSize[i]-1].suit);
      if(i+1 < MAX_FOUND)
        fprintf(outFile, " ");
    }
    fprintf(outFile, "\n");
    fprintf(outFile, "TABLEAU:\n");
    for(int i=COLUMN-1; i>=0; i--){
      if(!cSize[i]) fprintf(outFile, "| ");
      for(int j=0; j<tSize[i]; j++){
        fprintf(outFile, "%c%c ", tableau[i][j].rank, tableau[i][j].suit);
        if(cSize[i] == j+1)
          fprintf(outFile, "| ");
      }
      fprintf(outFile, "\n");
    }
    fprintf(outFile, "STOCK:\n");
    for(int i=0; i<wasteSize; i++)  fprintf(outFile, "%c%c ", waste[i].rank, waste[i].suit);
    fprintf(outFile, "| ");
    for(int i=stockSize-1; i>=0; i--) fprintf(outFile, "%c%c ", stock[i].rank, stock[i].suit);
    fprintf(outFile, "\nMOVES:\n");
  }
  else{
    /********************** Human-readable format ************************/
    fprintf(outFile, "Foundations:\n");
    for(int i=0; i<MAX_FOUND; i++){
      fprintf(outFile, "%c%c", found[i][fSize[i]-1].rank, found[i][fSize[i]-1].suit);
      if(i+1 < MAX_FOUND)
        fprintf(outFile, " ");
    }
    fprintf(outFile, "\n");
    fprintf(outFile, "Tableau:\n");
    int maxRows = 0;
    for(int i=0; i<COLUMN; i++)
      if(maxRows < tSize[i])
        maxRows = tSize[i];
    for(int i=0; i<maxRows; i++){
      for(int j=0; j<COLUMN; j++){
        if(tSize[j] <= i) fprintf(outFile, "..");
        else if(i < cSize[j]) fprintf(outFile, "##");
        else fprintf(outFile, "%c%c", tableau[j][i].rank, tableau[j][i].suit);
        if(j+1 < COLUMN) fprintf(outFile," ");
      }
      fprintf(outFile, "\n");
    }
    fprintf(outFile, "Waste top\n");
    int t = turns;
    if(t > wasteSize) t = wasteSize;
    if(!t) fprintf(outFile, "(empty)\n");
    else{
      for(int i=wasteSize-t; i<wasteSize; i++){
        fprintf(outFile, "%c%c", waste[i].rank, waste[i].suit);
        if(i+1 < t)
          fprintf(outFile, " ");
      }
      fprintf(outFile, "\n");
    }
  }
  
  /* **************** Calculating outputs **************** */
  /*
  int covered = 0;
  for(int i=0; i<COLUMN; i++)
      covered += cSize[i];
  
  printf("Input file is valid\n");
  printf("%d covered cards\n", covered);
  printf("%d stock cards\n", stockSize);
  printf("%d waste cards\n", wasteSize);
  
  
  printf("Printing out tableau:\n");
  for(int i=0; i<COLUMN; i++){
    printf("Column %d:\n", i);
    for(int j=0; j<tSize[i]; j++){
      struct Card a = tableau[i][j];
      printf("suit: %c, rank: %c, isuit: %d, irank: %d, color: %d\n", a.suit, a.rank, a.isuit, a.irank, a.color);
    }
    printf("Column Seperator is at: %d\n", cSize[i]);
  }
  printf("\nPrinting out waste:\n");
  for(int i=0; i<wasteSize; i++){
    struct Card a = waste[i];
    printf("suit: %c, rank: %c, isuit: %d, irank: %d, color: %d\n", a.suit, a.rank, a.isuit, a.irank, a.color);
  }
  printf("\nPrinting out stock:\n");
  for(int i=0; i<stockSize; i++){
    struct Card a = stock[i];
    printf("suit: %c, rank: %c, isuit: %d, irank: %d, color: %d\n", a.suit, a.rank, a.isuit, a.irank, a.color);
  }
  printf("\nPrinting out foundation:\n");
  for(int i=0; i<MAX_FOUND; i++){
    printf("foundation of suit: %d\n", i);
    for(int j=0; j<fSize[i]; j++){
      struct Card a = found[i][j];
      printf("suit: %c, rank: %c, isuit: %d, irank: %d, color: %d\n", a.suit, a.rank, a.isuit, a.irank, a.color);
    }
  }
  */
  
  return 0;
}
