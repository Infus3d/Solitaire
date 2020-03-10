#include "stdio.h"
#include "part1.h"

/* Simulation of a Klondike Solitaire (not finished yet)
 * @param argc: number of the command line arguments
 * @param argv: array of strings that represent command line arguments
 * @return 0 if the program is successfully executed
 * */
int main(int argc, char* argv[]){
    FILE* inFile;
    char* infileName;
    int turns, limit, lcount;
    if(argc == 1)
        inFile = stdin;
    else
        inFile = fopen(argv[1], "r"), infileName = argv[1];
    
    if(!inFile)
        return 0 * fprintf(stderr, "Input is invalid: File %s does not exist!", infileName);
    
    struct Card tableau[COLUMN][MAX_CARD], waste[MAX_CARD], stock[MAX_CARD], found[MAX_FOUND][MAX_CARD];
    int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
    
    if(!part1(inFile, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit, &lcount))
        return 0;
    
    /***************** Calculating outputs *****************/
    int covered = 0;
    for(int i=0; i<COLUMN; i++)
        covered += cSize[i];
    
    printf("Input file is valid\n");
    printf("%d covered cards\n", covered);
    printf("%d stock cards\n", stockSize);
    printf("%d waste cards\n", wasteSize);
    
    /*
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
