#include "part1.h"
#include "part2.h"
#include "stdio.h"
#include "string.h"

/* Returns the minimum of the two input parameters
 * @param x: first integer to compare
 * @param y: second integer to compare
 * @return the value of them minimum of the two given integers
 * */
int min(int x, int y){
  return x < y ? x : y;
}

int part2(FILE* inFile, int maxMoves, struct Card tableau[][MAX_CARD], int tSize[COLUMN], int cSize[COLUMN], 
          struct Card waste[MAX_STOCK], int* wasteSize, struct Card stock[MAX_STOCK], int* stockSize, struct Card found[MAX_FOUND][MAX_CARD], 
          int fSize[MAX_FOUND], int* turns, int* limit, int* lcount){
  
  //keeping the # of curline
  int lineCount = *lcount;
  char A, B;
  int i = 0;
  
  while(i < maxMoves){
    if(!hasNext(inFile, &lineCount))  break;
    char temp[MAX_CHAR];
    int len = getNext(inFile, temp);
    
    if(temp[0] == '#'){
      skipLine(inFile);
      continue;
    }
    // printf("%d %s '%d' '%d'\n", lineCount, temp, (int)temp[0], (int)temp[1]);
    
    if(len == 1){
      if(temp[0] == '.' || temp[0] == 'r')
        A = temp[0];
      else{
        fprintf(stderr, "Error near line %d: invalid format\n", lineCount);
        return 0;
      }
    }
    else if(len == 4){
      if((temp[0] == 'w' || ('1' <= temp[0] && temp[0] <= '7')) && temp[1] == '-' 
          && temp[2] == '>' && (temp[3] == 'f' || ('1' <= temp[3] && temp[3] <= '7')))
        A = temp[0], B = temp[3];
      else{
        fprintf(stderr, "Error near line %d: invalid format\n", lineCount);
        return 0;
      }
    }
    else{
      fprintf(stderr, "Error near line %d: invalid format\n", lineCount);
      return 0;
    }
    
    if(A == '.'){  //check for turn-over
      if(!(*stockSize)){
        printf("Move %d is illegal: %c\n", i+1, A);
        return -1;
      }
      int t = min(*stockSize, *turns);
      while(t--) waste[(*wasteSize)++] = stock[--(*stockSize)];
    }
    else if(A == 'r'){ //check for reset
      if(!(*limit) || (*stockSize)){
        printf("Move %d is illegal: %c\n", i+1, A);
        return -1;
      }
      while(*wasteSize) stock[(*stockSize)++] = waste[--(*wasteSize)];
      if(*limit < 10) (*limit)--;
    }
    else{ //check for %c->%c
      struct Card from;
      struct Card onto;
      
      if(A == 'w'){ // A is 'w'
        if(!(*wasteSize)){
          printf("Move %d is illegal: %c->%c\n", i+1, A, B);
          return -1;
        }
        from = waste[(*wasteSize)-1];
        
        if(B == 'f'){ // B is 'f'
          onto = found[from.isuit][fSize[from.isuit]-1];
          if(from.irank != onto.irank+1){
            printf("Move %d is illegal: %c->%c\n", i+1, A, B);
            return -1;
          }
          found[from.isuit][fSize[from.isuit]++] = waste[--(*wasteSize)];
        }
        else{ //B is '0'...'7'
          int bCol = B-'1';
          if(tSize[bCol])
            onto = tableau[bCol][tSize[bCol]-1];
          else
            onto.rank = '_', onto.suit = 'u', onto.irank = 14, onto.isuit = -1, onto.color = 0;
          
          if(from.color != onto.color && from.irank == onto.irank-1)
            tableau[bCol][tSize[bCol]++] = waste[--(*wasteSize)];
          else{
            printf("Move %d is illegal: %c->%c\n", i+1, A, B);
            return -1;
          }
        }
      }
      else{ //A is '0'...'7'
        int aCol = A-'1';
        if(!tSize[aCol]){
          printf("Move %d is illegal: %c->%c\n", i+1, A, B);
          return -1;
        }
        if(B == 'f'){  // B is 'f'
          from = tableau[aCol][tSize[aCol]-1];
          onto = found[from.isuit][fSize[from.isuit]-1];
          
          if(from.irank != onto.irank+1){
            printf("Move %d is illegal: %c->%c\n", i+1, A, B);
            return -1;
          }
          found[from.isuit][fSize[from.isuit]++] = tableau[aCol][--tSize[aCol]];
          if(tSize[aCol] == cSize[aCol] && cSize[aCol])
            cSize[aCol]--;
        }
        else{ // B is '0'...'7'
          int bCol = B-'1', ok = -1;
          if(tSize[bCol])
            onto = tableau[bCol][tSize[bCol]-1];
          else
            onto.rank = '!', onto.suit = 'u', onto.irank = 14, onto.isuit = -1, onto.color = 0;
          
          for(int j=cSize[aCol]; j<tSize[aCol]; j++){
            struct Card cur = tableau[aCol][j];
            // printf("%d %c %c, cur:%d  onto:%d, %d %d\n", j, cur.rank, cur.suit, cur.irank, onto.irank, cur.color, onto.color);
            if(onto.color != cur.color && onto.irank == cur.irank+1){
              ok = j;
              break;
            }
          } //puts("********");
          if(ok == -1){
            printf("Move %d is illegal: %c->%c\n", i+1, A, B);
            return -1;
          }
          for(int j=ok; j<tSize[aCol]; j++) 
            tableau[bCol][tSize[bCol]++] = tableau[aCol][j];
          tSize[aCol] = ok;
          cSize[aCol] -= (cSize[aCol] && cSize[aCol] == tSize[aCol]) ? 1 : 0;
        }
      }
    }
    i++;
  }
  printf("Processed %d moves, all valid\n", i);
  
  return 1;
}
