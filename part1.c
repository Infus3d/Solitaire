#include "part1.h"
#include "stdio.h"
#include "string.h"

#define MAX_CHAR 20

// Useful constants
const char keys[][MAX_CHAR] = {"RULES:", "FOUNDATIONS:", "TABLEAU:", "STOCK:", "MOVES:"};
const char ranks[] = "_A23456789TJQK";
const char suits[] = "cdhs";

// Keeps track of the line number to report error
int lineCount = 1;

// Global variables to keep track of the game mode: turn number and limit of resetting the waste back to the stock
int turns, limit;

// Global helper array
int allCard[4][15];

// Global arrays to keep track of the tableau, waste, stock and foundation respectively
struct Card tableau[COLUMN][MAX_CARD], waste[MAX_CARD], stock[MAX_CARD], found[MAX_FOUND][MAX_CARD];

// Global arrays to keep track of the size of the tableau columns, tableau seperators, waste, stock and foundation respectively
int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];

/* Checks if the given character is a space (either one of ' ', '\t',
 * '\r' and '\n')
 * @param ch: given character
 * @return 1 if the character is a space, 0 otherwise
 * */
int isSpace(int ch){
    return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
}

/* Checks if there exists any non-space character in the given file buffer
 * @param inFile: file to read the data from
 * @return 1 if the buffer contains non-space character, 0 otherwise
 * */
int hasNext(FILE* inFile, int* lcount){
    while(1){
        if(feof(inFile))
            return 0;
        
        int nxt = fgetc(inFile);
        
        if(feof(inFile))
          return 0;
        if(!isSpace(nxt)){
            ungetc(nxt, inFile);
            return 1;
        }
        
        if(nxt == '\n') (*lcount)++;
    }
}

/* Scans in the next non-space sequence of characters into the provided
 * string (str). Stops scanning into the string if the length of the word
 * exceeds MAX_CHAR-1. Assigns \0 to the end of the string (null termination)
 * @param inFile: given file to read data from
 * @param str: given string to scan data into
 * @return the size of the scanned word
 * */
int getNext(FILE* inFile, char str[]){
    int ptr = 0;
    while(ptr < MAX_CHAR-1){
        if(feof(inFile))
            break;
        
        int nxt = fgetc(inFile);
        if(isSpace(nxt)){
            ungetc(nxt, inFile);
            break;
        }
        
        str[ptr++] = (char)nxt;
    }
    
    str[ptr] = 0;
    return ptr;
}

/* Checks if the two given strings are equal
 * @param a: given first string
 * @param b: given second string
 * @return 1 if the strings are equal, 0 otherwise
 * */
int isEqual(char a[], const char b[]){
    if(strlen(a) != strlen(b))
        return 0;
    
    for(int i=0; i<strlen(a); i++)
        if(a[i] != b[i])
            return 0;
    
    return 1;
}

/* Skips all the characters until reaches a new line '\n', in which case
 * ungets it and retains '\n' untouched in buffer
 * @param inFile: given file to read from
 * */
void skipLine(FILE* inFile){
    while(!feof(inFile)){
        int nxt = fgetc(inFile);
        if(nxt == '\n'){
            ungetc(nxt, inFile);
            break;
        }
    }
}

/* Checks if the given string is a digit
 * @param a: given string
 * @return 1 if the string is between 0 <= string <= 9, 0 otherwise
 * */
int isDigit(char a[]){
    if(strlen(a) != 1)
        return 0;
    
    return '0' <= a[0] && a[0] <= '9';
}

/*
 * Evaluates the RULES module
 * @param inFile: given file to read data from
 * @return 1 if the given module is valid, 0 otherwise
 * */
int rules(FILE* inFile){
    int counter = 0;
    while(1){
        if(!hasNext(inFile, &lineCount)){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, counter == 0 ?
                "turn" : counter == 1 ? "1 or 3" : counter == 2 ? "unlimited or limit" : "limit number([0...9])");
            return 0;
        }
        char temp[MAX_CHAR];
        getNext(inFile, temp);
        
        if(temp[0] == '#'){
            skipLine(inFile);
            continue;
        }
        if(counter == 0){
            if(!isEqual(temp, "turn")){
                fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "turn");
                return 0;
            }
        }
        else if(counter == 1){
            if(!isEqual(temp, "1") && !isEqual(temp, "3")){
                fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "1 or 3");
                return 0;
            }
            turns = temp[0]-'0';
        }
        else if(counter == 2){
            if(isEqual(temp, "unlimited")){
                limit = 10; // 10 indicates unlimited
                return 1;
            }
            else if(!isEqual(temp, "limit")){
                fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "limit or unlimited");
                return 0;
            }
        }
        else{
            if(!isDigit(temp)){
                fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "limit number([0...9])");
                return 0;
            }
            
            limit = temp[0]-'0';
            
            return 1;
        }
        counter++;
    }
    
    return 0;
}

/* Returns the suit of the card
 * @param a: given suit of the card in char format
 * @return the corresponsing index of occurrence in "cdhs", -1 if not found
 * */
int getSuit(char a){
    for(int i=0; i<strlen(suits); i++)
        if(suits[i] == a)
            return i;
    return -1;
}
/* Returns the rank of the card
 * @param a: given rank of the card in char format
 * @return the corresponsing index of occurrence in "_A23456789TJQK", -1 if not found
 * */
int getRank(char a){
    for(int i=0; i<strlen(ranks); i++)
        if(ranks[i] == a)
            return i;
    return -1;
}

/* Returns the color of the suit
 * @param a: suit of the card
 * @return -1 if black, 1 otherwise
 * */
int getColor(char a){
    return a == 'c' || a == 's' ? -1 : 1;
}

/* Checks for validity of the card
 * @param a: given card in form of a string
 * @return 1 if the given string is indeed card, 0 otherwise
 * */
int isValidCard(char a[]){
    if(strlen(a) != 2)
        return 0;
    
    return ~getSuit(a[1]) && ~getRank(a[0]);
}
/* Checks for duplicates
 * @param a: given card
 * @return true if the given card has been seen before, false otherwise
 * */
int isDuplicate(struct Card a){
    allCard[a.isuit][a.irank]++;
    if(allCard[a.isuit][a.irank] > 1)
        return 1;
    return 0;
}

/*
 * Evaluates the FOUNDATION module
 * @param inFile: given file to read data from
 * @return 1 if the given module is valid, 0 otherwise
 * */
int foundation(FILE* inFile){
    int counter = 0;
    while(counter < 4){
        if(!hasNext(inFile, &lineCount)){
            fprintf(stderr, "Error near line %d: Expecting %s %d\n", lineCount, "card", counter);
            return 0;
        }
        char temp[MAX_CHAR];
        getNext(inFile, temp);
        
        if(temp[0] == '#'){
            skipLine(inFile);
            continue;
        }
        if(!isValidCard(temp) || counter != getSuit(temp[1])){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "card");
            return 0;
        }
        
        for(int i=0; i<=getRank(temp[0]); i++){
            found[counter][i].suit = temp[1];
            found[counter][i].rank = ranks[i];
            found[counter][i].isuit = getSuit(temp[1]);
            found[counter][i].irank = i;
            found[counter][i].color = getColor(temp[1]);
            if(isDuplicate(found[counter][i])){
                fprintf(stderr, "Error near line %d: Duplicate card %s\n", lineCount, temp);
                return 0;
            }
            fSize[counter]++;
        }
        counter++;
    }
    
    return 1;
}
/*
 * Evaluates the TABLEAU module
 * @param inFile: given file to read data from
 * @return 1 if the given module is valid, 0 otherwise
 * */
int table(FILE* inFile){
    int counter = 6;
    int vis[COLUMN];
    for(int i=0; i<=counter; i++)
        vis[i] = 0;
    
    while(counter > -1){
        int before = lineCount;
        if(!hasNext(inFile, &lineCount)){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "tableau card/seperator");
            return 0;
        }
        counter -= ((lineCount - before) && vis[counter]) ? 1 : 0;
        if(counter < 0) break;
        
        char temp[MAX_CHAR];
        getNext(inFile, temp);
        // printf("%d %d %s\n", counter, lineCount, temp);
        
        if(temp[0] == '#'){
            skipLine(inFile);
            continue;
        }
        if(lineCount - before == 1 && !vis[counter+1]){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount-1, "seperator");
            return 0;
        }
        if((!isEqual(temp, "|") && !isValidCard(temp)) || (isEqual(temp, "|") && vis[counter])){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "valid tableau card/seperator");
            return 0;
        }
        
        if(temp[0] == '|'){
            cSize[counter] = tSize[counter];
            vis[counter] = 1;
            continue;
        }
        
        tableau[counter][tSize[counter]].suit = temp[1];
        tableau[counter][tSize[counter]].rank = temp[0];
        tableau[counter][tSize[counter]].isuit = getSuit(temp[1]);
        tableau[counter][tSize[counter]].irank = getRank(temp[0]);
        tableau[counter][tSize[counter]].color = getColor(temp[1]);
        if(isDuplicate(tableau[counter][tSize[counter]])){
                fprintf(stderr, "Error near line %d: Duplicate card %s\n", lineCount, temp);
                return 0;
            }
        tSize[counter]++;
    }
    
    return 1;
}

/*
 * Evaluates the STOCK module
 * @param inFile: given file to read data from
 * @return 1 if the given module is valid, 0 otherwise
 * */
int stockAndWaste(FILE* inFile){
    int vis = 0;
    while(1){
        if(!hasNext(inFile, &lineCount)){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "MOVES:");
            return 0;
        }
        char temp[MAX_CHAR];
        getNext(inFile, temp);
        
        if(temp[0] == '#'){
            skipLine(inFile);
            continue;
        }
        if(isEqual(temp, keys[4])){  
            break;
        }
        if((!isEqual(temp, "|") && !isValidCard(temp)) || (isEqual(temp, "|") && vis)){
            fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, "valid card/seperator");
            return 0;
        }
        
        if(isEqual(temp, "|")){
            vis = 1;
            continue;
        }
        
        if(vis){
            stock[stockSize].suit = temp[1];
            stock[stockSize].rank = temp[0];
            stock[stockSize].isuit = getSuit(temp[1]);
            stock[stockSize].irank = getRank(temp[0]);
            stock[stockSize].color = getColor(temp[1]);
            if(isDuplicate(stock[stockSize])){
                fprintf(stderr, "Error near line %d: Duplicate card %s\n", lineCount, temp);
                return 0;
            }
            stockSize++;
        }
        else{
            waste[wasteSize].suit = temp[1];
            waste[wasteSize].rank = temp[0];
            waste[wasteSize].isuit = getSuit(temp[1]);
            waste[wasteSize].irank = getRank(temp[0]);
            waste[wasteSize].color = getColor(temp[1]);
            if(isDuplicate(waste[wasteSize])){
                fprintf(stderr, "Error near line %d: Duplicate card %s\n", lineCount, temp);
                return 0;
            }
            wasteSize++;
        }
    }
    if(stockSize)
        for(int i=0; i<stockSize/2; i++){
            struct Card tmp = stock[i];
            stock[i] = stock[stockSize-i-1];
            stock[stockSize-i-1] = tmp;
        }
    
    return 1;
}

int part1(FILE* inFile, struct Card gtableau[][MAX_CARD], int gtSize[COLUMN], int gcSize[COLUMN], struct Card gwaste[MAX_CARD], int* gwasteSize,
            struct Card gstock[MAX_CARD], int* gstockSize, struct Card gfound[MAX_FOUND][MAX_CARD], int gfSize[MAX_FOUND], 
            int* gturns, int* glimit, int* lcount){
    
    char str[MAX_CHAR];
    int keyCn = 0; //key counter
    while(keyCn < 5 && !feof(inFile)){
        if(!hasNext(inFile, &lineCount))    break;
        getNext(inFile, str);
        
        if(str[0] == '#'){   
            skipLine(inFile);
            continue;
        }
        if(!isEqual(str, keys[keyCn])){
            fprintf(stderr, "Errore near line %d: Expecting %s\n", lineCount, keys[keyCn]);
            return 0;
        }
        
        if(keyCn == 0){
            if(!rules(inFile))
                return 0;
        }
        else if(keyCn == 1){
            if(!foundation(inFile))
                return 0;
        }
        else if(keyCn == 2){
            if(!table(inFile))
                return 0;
        }
        else if(keyCn == 3){
            if(!stockAndWaste(inFile))
                return 0;
            keyCn++;
        }
        keyCn++;
    }
    
    if(keyCn < 5){
        fprintf(stderr, "Error near line %d: Expecting %s\n", lineCount, keys[keyCn]);
        return 0;
    }
    
    /****************** Checking for pile validity *******************/
    for(int i=0; i<COLUMN; i++){
        if(cSize[i] == tSize[i] && cSize[i] > 0){
            fprintf(stderr, "Error in the tableau on column %d: covered cards must be followed by at least one uncovered card\n", i);
            return 0;
        }
        for(int j=cSize[i]; j<tSize[i]-1; j++)
            if(tableau[i][j].irank <= tableau[i][j+1].irank || tableau[i][j].color == tableau[i][j+1].color){
                fprintf(stderr, "Error in the tableau on column %d: invalid pile\n", i);
                return 0;
            }
    }
    /******************** Checking for missing cards ***********************/
    char missing[MAX_CARD][3]; int missingSize = 0;
    for(int i=0; i<strlen(suits); i++)
        for(int j=1; j<strlen(ranks); j++){
            if(!allCard[i][j]){
                missing[missingSize][1] = suits[i];
                missing[missingSize][0] = ranks[j];
                missing[missingSize][2] = 0;
                missingSize++;
            }
        }
    
    if(missingSize){
        fprintf(stderr, "There are %d missing cards!\n", missingSize);
        for(int i=0; i<missingSize; i++)
            fprintf(stderr, "%s ", missing[i]);
        return 0;
    }
    
    /**************** Copying back to the original given variables ********************/
    for(int i=0; i<COLUMN; i++){
      for(int j=0; j<tSize[i]; j++)
        gtableau[i][j] = tableau[i][j];
      gcSize[i] = cSize[i];
      gtSize[i] = tSize[i];
    }
    
    (*gwasteSize) = wasteSize;
    for(int i=0; i<wasteSize; ++i)
        gwaste[i] = waste[i];
    
    (*gstockSize) = stockSize;
    for(int i=0; i<stockSize; i++)
        gstock[i] = stock[i];
    
    for(int i=0; i<MAX_FOUND; i++){
        for(int j=0; j<fSize[i]; j++)
            gfound[i][j] = found[i][j];
        gfSize[i] = fSize[i];
    }
    (*gturns) = turns;
    (*glimit) = limit;
    (*lcount) = lineCount;
    
    return 1;
}
