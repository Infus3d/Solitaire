#include "stdio.h"
#include "part1.h"

/* Simulation of a Klondike Solitaire (not finished yet)
 * @param argc: number of the command line arguments
 * @param argv: array of strings that represent command line arguments
 * @return 0 if the program is successfully executed
 * */
int main(int argc, char* argv[]){
    FILE* inFile;
    int turns, limit;
    if(argc == 1)
        inFile = stdin;
    else
        inFile = fopen(argv[1], "r");
    
    if(!inFile)
        return 0 * fprintf(stderr, "Input is invalid:\n File does not exist!");
    
    struct Card tableau[COLUMN][MAX_CARD], waste[MAX_CARD], stock[MAX_CARD], found[MAX_FOUND][MAX_CARD];
    int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
    
    if(!part1(inFile, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit))
        return 0;       // Assuming part1 already printed the message
    
    
    return 0;
}
