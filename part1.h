#ifndef PART1_H
#define PART1_H
    #include "stdio.h"
    
    /* Useful global constants.
     * */
    #define MAX_CARD 20
    #define MAX_STOCK 55
    #define MAX_FOUND 4
    #define COLUMN 7
    #define MAX_CHAR 20
    
    /* Simple Card struct that has suit, rank and color as member variables.
     * Also contains the integer version of suit (isuit) and rank (irank).
     * */
    struct Card{
        char suit;
        char rank;
        int isuit;
        int irank;
        int color;
    };
    
    //Documentation for these functions are provided in part1.c
    int isSpace(int ch);
    int hasNext(FILE* inFile, int* lcount);
    int getNext(FILE* inFile, char str[]);
    int isEqual(char a[], const char b[]);
    void skipLine(FILE* inFile);
    int isDigit(char a[]);
    int getSuit(char a);
    int getRank(char a);
    int getColor(char a);
    int isValidCard(char a[]);
    
    /* The function is the first part of the COMS327S20 Project.
     * Reads Klondike Solitaire game's data from the file provided and parses
     * it to check if the given data is valid or not. Moreover, suggests error
     * lines to make it easy for the users to locate their mishaps. Also checks
     * for duplicate and missing cards and for the validity of the piles in tableau.
     * If there is no errors in the file, it outputs the number of covered, in-stock
     * and in-waste cards. Fills the game details into the provided parameters.
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
     * @return 1 if the file has been successfully read, 0 otherwise
     * */
    int part1(FILE* inFile, struct Card tableau[][MAX_CARD], int tSize[COLUMN], int cSize[COLUMN], struct Card waste[MAX_STOCK], int* wasteSize,
              struct Card stock[MAX_STOCK], int* stockSize, struct Card found[][MAX_CARD], int fSize[MAX_FOUND], int* turns, 
              int* limit, int* lcount);
#endif
