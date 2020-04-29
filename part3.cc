#include "iostream"
#include "unordered_map"
#include "locale.h"
#include "ctime"
#include "algorithm"
#include "assert.h"

#include "part1.h"
#include "part3.hh"

using namespace std;

gamestate::gamestate(Card _tableau[][MAX_CARD], Card _waste[MAX_STOCK], Card _stock[MAX_STOCK], Card _found[][MAX_CARD], 
              int _tSize[COLUMN], int _cSize[COLUMN], int _wasteSize, int _stockSize, int _fSize[MAX_FOUND], int _turns, int _limit){
  
  for(int i=0; i<COLUMN; i++){
    for(int j=0; j<_tSize[i]; j++)
      tableau[i][j] = _tableau[i][j];
    cSize[i] = _cSize[i];
    tSize[i] = _tSize[i];
  }
  
  wasteSize = _wasteSize;
  for(int i=0; i<_wasteSize; ++i)
    waste[i] = _waste[i];
  
  stockSize = _stockSize;
  for(int i=0; i<_stockSize; i++)
    stock[i] = _stock[i];
  
  for(int i=0; i<MAX_FOUND; i++){
    for(int j=0; j<_fSize[i]; j++)
      found[i][j] = _found[i][j];
    fSize[i] = _fSize[i];
  }
  turns = _turns;
  limit = _limit;
}

string gamestate::toString(){
  string ret = to_string(limit) + "\n";
  for(int i=0; i<MAX_FOUND; i++){      // given that the game state is always valid
    ret += found[i][fSize[i]-1].rank;  // we can just omit the foundation part when
    ret += found[i][fSize[i]-1].suit;  // we hash becuse they can have only 1 configuration
    if(i+1 < MAX_FOUND) ret += " ";    // for any given configuration of tableau and stock/waste
  }
  ret += "\n";
  for(int i=COLUMN-1; i>=0; i--){
    if(!cSize[i]) ret += "| ";
    for(int j=0; j<tSize[i]; j++){
      ret += tableau[i][j].rank;
      ret += tableau[i][j].suit;  ret += " ";
      if(cSize[i] == j+1) ret += "| ";
    }
    ret += "\n";
  }
  for(int i=0; i<wasteSize; i++)  
    ret += waste[i].rank, ret += waste[i].suit, ret += " ";
  ret += "|";
  for(int i=stockSize-1; i>=0; i--) 
    ret += " ", ret += stock[i].rank, ret += stock[i].suit;
  
  return ret;
}

/* Helper function that determines if the game is winnable in current
 * given game state. Game is winnable if all cards are in the foundation or
 * if there are no covered cards, no stock cards and at most one waste card.
 * @param G: given game state to check
 * @return true if the game is winnable in the given game state, false otherwise
 * */
bool isWinnable(const gamestate& G){
  bool winnable = true;
  for(int i=0; i<MAX_FOUND; i++)
    winnable &= (G.fSize[i] == 14);
  if(winnable) return true;
  
  winnable = true;
  for(int i=0; i<COLUMN; i++)
    winnable &= (G.cSize[i] == 0);
  winnable &= (G.stockSize == 0);
  winnable &= (G.wasteSize < 2);
  return winnable;
}

/* Struct that aids with undo parameters. Used to save the minimal details
 * about the configuration of the gamestate before making a move so that
 * the move can be undone.
 * */
struct undoHelper{
  int cSize, okLen, stockSize;
  /* Constuctor for undoHelper. Simply assigns the given numbers to
   * member variables.
   * */
  undoHelper(int _cSize, int _okLen, int _stockSize){
    cSize = _cSize; okLen = _okLen; stockSize = _stockSize;
  }
};

/* Helper function that undoes the most recent move made by isValid
 * function with the help of the passed undoHelper variable. The most 
 * recent move is passed with characters A and B where they can be in
 * form 'A->B'. If the move is '.' or 'r', only the value of character A
 * is considered.
 * @param H: the given game state to undo the move in
 * @param A: the first character that defines a move
 * @param B: the second character that defines a move
 * @param h: the helper variable that enables the reversing
 * */
void undo_IsValid(gamestate& H, char A, char B, const undoHelper& h){
  if(A == '.'){
    int t = min(h.stockSize, H.turns);
    H.wasteSize -= t;
    H.stockSize += t;
  }
  else if(A == 'r'){
    H.wasteSize = H.stockSize;
    H.stockSize = 0;
    if(H.limit < 10) (H.limit)++;
  }
  else{
    Card from;
    if(A == 'w'){
      from = H.waste[H.wasteSize++];
      if(B == 'f')
        H.fSize[from.isuit]--;
      else
        H.tSize[B-'1']--;
    }
    else{
      short aCol = A-'1';
      from = H.tableau[aCol][H.tSize[aCol]];
      if(B == 'f'){
        H.tSize[aCol]++;
        H.fSize[from.isuit]--;
        H.cSize[aCol] += h.cSize;
      }
      else{
        short bCol = B-'1';
        H.tSize[bCol] -= h.okLen;
        H.cSize[aCol] += h.cSize;
        H.tSize[aCol] += h.okLen;
      }
    }
  }
}

/* Function that determines if the provided move in form of 'A' -> 'B',
 * '.' or 'r' is valid for the given game state. If it is, the function
 * proceeds with that move and returns true. If not, it simply returns false;
 * The function also keeps track of the move so that it be undone later.
 * @param H: given game state to check
 * @param A: the first character that defines a move
 * @param B: the second character that defines a move
 * @param h: the helper variable that keeps track of the move
 * @return true if the move is valid for the given game state, false otherwise
 * */
bool isValid(gamestate& H, char A, char B, undoHelper& h){
  if(A == '.'){  //check for turn-over
    if(!(H.stockSize))  return false;
    int t = min(H.stockSize, H.turns);
    h.stockSize = H.stockSize;  //to help undo it
    while(t--) H.waste[(H.wasteSize)++] = H.stock[--(H.stockSize)];
  }
  else if(A == 'r'){ //check for reset
    if(!(H.limit) || (H.stockSize)) return false;
    while(H.wasteSize) H.stock[(H.stockSize)++] = H.waste[--(H.wasteSize)];
    if(H.limit < 10) (H.limit)--;
  }
  else{ //check for %c->%c
    Card from;
    Card onto;
    
    if(A == 'w'){ // A is 'w'
      if(!(H.wasteSize))  return false;
      from = H.waste[(H.wasteSize)-1];
      
      if(B == 'f'){ // B is 'f'
        onto = H.found[from.isuit][H.fSize[from.isuit]-1];
        if(from.irank != onto.irank+1)  return false;
        H.found[from.isuit][H.fSize[from.isuit]++] = H.waste[--(H.wasteSize)];
      }
      else{ //B is '0'...'7'
        int bCol = B-'1';
        if(H.tSize[bCol])
          onto = H.tableau[bCol][H.tSize[bCol]-1];
        else
          onto.rank = '_', onto.suit = 'u', onto.irank = 14, onto.isuit = -1, onto.color = 0;
        
        if(from.color != onto.color && from.irank == onto.irank-1)
          H.tableau[bCol][H.tSize[bCol]++] = H.waste[--(H.wasteSize)];
        else  
          return false;
      }
    }
    else{ //A is '0'...'7'
      int aCol = A-'1';
      if(!H.tSize[aCol])  return false;
      if(B == 'f'){  // B is 'f'
        from = H.tableau[aCol][H.tSize[aCol]-1];
        onto = H.found[from.isuit][H.fSize[from.isuit]-1];
        
        if(from.irank != onto.irank+1)  return false; 
        H.found[from.isuit][H.fSize[from.isuit]++] = H.tableau[aCol][--H.tSize[aCol]];
        if(H.tSize[aCol] == H.cSize[aCol] && H.cSize[aCol])
          H.cSize[aCol]--, h.cSize = 1;
      }
      else{ // B is '0'...'7'
        int bCol = B-'1', ok = -1;
        if(H.tSize[bCol])
          onto = H.tableau[bCol][H.tSize[bCol]-1];
        else
          onto.rank = '!', onto.suit = 'u', onto.irank = 14, onto.isuit = -1, onto.color = 0;
        
        for(int j=H.cSize[aCol]; j<H.tSize[aCol]; j++){
          Card cur = H.tableau[aCol][j];
          if(onto.color != cur.color && onto.irank == cur.irank+1){
            ok = j;
            break;
          }
        }
        if(ok == -1)  return false;
        h.okLen = H.tSize[aCol] - ok;
        for(int j=ok; j<H.tSize[aCol]; j++) 
          H.tableau[bCol][H.tSize[bCol]++] = H.tableau[aCol][j];
        H.tSize[aCol] = ok;
        H.cSize[aCol] -= h.cSize = (H.cSize[aCol] && H.cSize[aCol] == H.tSize[aCol] ? 1 : 0);
      }
    }
  }
  return true;
}

bool isValid(gamestate& H, char A, char B){
  undoHelper h = undoHelper(0, 0, 0);
  return isValid(H, A, B, h);
}

/* Function that determines if the move is "safe" for the given game state.
 * The move is said to be "safe" if the second character is 'f' (meaning
 * the move puts a card onto one of the foundation piles) and 
 * if (1) the card is not being moved from the waste pile,unless we are 
 * turning 1 card over at a time; and (2) the ranks of the foundation cards
 * on the opposite color are at least as large as the current rank of the 
 * foundation suit we are adding to.
 * @param G: given gamestate to check
 * @param A: the first character that defines a move
 * @param B: the second character that defines a move
 * @return true if the move is safe, false otherwise
 * */
bool isSafe(const gamestate& G, char A, char B){
  if(A == 'w' && G.turns != 1) return false;
  Card from = A == 'w' ? G.waste[G.wasteSize] : G.tableau[A-'1'][G.tSize[A-'1']];
  for(int i=0; i<MAX_FOUND; i++){
    Card cur = G.found[i][G.fSize[i]-1];
    if(cur.color != from.color && cur.rank < from.rank-1)
      return false;
  }
  return true;
}
//useful constants that help make a "move"
const char movesB[] = {'f', '1', '2', '3', '4', '5', '6', '7'};
const char misc[] = {'.', 'r'};
int minn = 1000;
vector<string> cur;

/* Dfs search that tries every possible move before reaching a "winnable" condition. If the cached
 * parameter is enabled, the function employs a cache (hashtable) that helps to reduce the overall
 * search time by keeping track of the game states: if the previous encounter of the current game
 * state had M moves left and it did not reach the winning state and it is greater than the current
 * moves left N (if M >= N) then the function returns since it is unnecessary to check further.
 * If forced parameter is enabled, the search forces the "safe" moves defined by the isSafe function.
 * Which may eventually lead to a faster search while it might increase the # of required to win. if vformat
 * variable is enabled, the function reports to stderr the number of checked configurations so far.
 * @param G: the current game state
 * @param hashT: hashtable that acts as a cache
 * @param ans: vector that contains the moves that lead to winning condition. The vector size is 0 if not winnable.
 * @param leftMoves: the number of moves left
 * @param vformat: if enabled, the function will report the progress
 * @param cached: if true, the function will employ a "cache"
 * @param forced: if true, the function will force "safe" moves
 * @param configCount: counter that keeps track of the # of configuration checked
 * */
bool rec(gamestate& G, unordered_map<string, int>& hashT, vector<string>& ans, int leftMoves, 
          bool vformat, bool cached, bool forced, int& configCount){
  
  if(isWinnable(G)) return true;
  if(!leftMoves) return false;
  
  if(cached){
    string t = G.toString();
    if(hashT[t] >= leftMoves)
      return false;
    hashT[t] = leftMoves;
  }
  
  configCount++;
  minn = leftMoves < minn ? leftMoves : minn;
  if(vformat && configCount % 76543 == 0){
    fprintf(stderr, "\r%'d configurations checked so far", configCount);
    fprintf(stderr, "and %d is the minimum so far", minn);
    fflush(stdout);
  }
  
  gamestate H = gamestate(G);
  undoHelper h = undoHelper(0, 0, 0);
  bool isFound = 0;


  char movesA[] = {'1', '2', '3', '4', '5', '6', '7', 'w'}; 
  sort(movesA, movesA+7, [&](const char& x, const char& y){
    return G.cSize[x-'1'] > G.cSize[y-'1'];
  });
  
  if(forced){
    for(int i=0; i<8; i++){
      h.okLen = h.stockSize = h.cSize = 0;
      if(isValid(H, movesA[i], movesB[0], h)){ 
        if(isSafe(H, movesA[i], movesB[0])){
          // string move = ""; move += movesA[i]; move += "->"; move += movesB[0]; //
          // cur.push_back(move); //
          isFound |= rec(H, hashT, ans, leftMoves-1, vformat, cached, forced, configCount);
          if(isFound){
            string move = ""; move += movesA[i]; move += "->"; move += movesB[0]; //
            ans.push_back(move);
            return isFound;
          }
          // cur.pop_back(); //
          return isFound;
        }
        undo_IsValid(H, movesA[i], movesB[0], h);
      }
    }
  }
  
  for(int i=0; i<2; i++){
    h.okLen = h.stockSize = h.cSize = 0;
    if(isValid(H, misc[i], misc[i], h)){
      // string move = ""; move += misc[i];//
      // cur.push_back(move); //
      
      isFound |= rec(H, hashT, ans, leftMoves-1, vformat, cached, forced, configCount);
      undo_IsValid(H, misc[i], misc[i], h);
              
      if(isFound){
        string move = ""; move += misc[i]; //
        ans.push_back(move);
        return isFound;
      }
      // cur.pop_back(); //
    }
  }
  
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      if(movesA[i] == movesB[j]) continue;
      h.okLen = h.stockSize = h.cSize = 0;
      if(isValid(H, movesA[i], movesB[j], h)){
        // string move = ""; move += movesA[i]; move += "->"; move += movesB[j]; //
        // cur.push_back(move); //
        
        isFound |= rec(H, hashT, ans, leftMoves-1, vformat, cached, forced, configCount); 
        undo_IsValid(H, movesA[i], movesB[j], h);
          
        if(isFound){
          string move = ""; move += movesA[i]; move += "->"; move += movesB[j]; //
          ans.push_back(move);
          return isFound;
        }
        // cur.pop_back(); //
      }
    }
  }
  
  return isFound;
}

bool part3(gamestate G, vector<string>& ans, int maxS, bool cached, bool forced, bool vformat){
  unordered_map<string, int> hashT;
  setlocale(LC_NUMERIC, "");
  
  if(vformat)
    fprintf(stderr, "Using DFS search\n");
  
  int configCount = 0;
  bool isFound = rec(G, hashT, ans, maxS, vformat, cached, forced, configCount);
  
  if(vformat){
    fprintf(stderr, "\nDone. Total of %'d configurations checked.\n", configCount);
    if(cached)  fprintf(stderr, "Cache had %d entries.\n", (int)hashT.size());
    fprintf(stderr, "Time elapsed: %.6lf s.\n", 1.0 * clock() / CLOCKS_PER_SEC);
    // fprintf(stderr, "Time spent on hashtable: %.6lf s.\n", totT);
  }
  
  /*
  int cn = 0;
  printf("# Possible sequence:\n"); fflush(stdout); // 
  for(string i : cur) //
    cout << i << " \n"[(++cn)%5 == 0]; //
  fflush(stdout);
  
  vector<string> v;
  unordered_map<string, int> mep;
  gamestate S = gamestate(G);
  undoHelper h = undoHelper(0, 0, 0);
  mep[G.toString()] = -1; v.push_back(S.toString());
  for(int i=0; i<(int)cur.size(); i++){
    string t = cur[i];
    char A, B;
    if((int)t.size() == 1) A = B = t[0];
    else  A = t[0], B = t[3];
    if(!isValid(S, A, B, h)){
      printf("Move %d not valid! ", i+1);
      cout << t << endl;
      break;
    }
    string hesh = S.toString();
    if(mep[hesh] != 0){
      cout << "State " << hesh << " has been seen before on move " << mep[hesh] << " and we are currently at move " << i+1 << endl;
      break;
    }
    mep[hesh] = i+1;
    v.push_back(hesh);
    // cout << hesh << endl;
  }
  
  for(int i=0; i<(int)v.size(); i++)
    for(int j=i+1; j<(int)v.size(); j++)
      if(v[i] == v[j]){
        printf("These two bois are the same: %d, %d\n", i, j);
      }
  
  isFound = 0;
  */
  return isFound;
}
