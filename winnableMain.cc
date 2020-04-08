#include "iostream"
#include "stdlib.h"
#include "string.h"
#include "algorithm"

#include "part1.h"
#include "part2.h"
#include "part3.hh"

using namespace std;

/* Main for COMS327S20 Project. Reads a game cofiguration from a file and
 * prints out a sequence of moves that leads to a winning state to stdout. 
 * Reads the filename to read the game state from the command line 
 * (stdout if omitted). Reports to stderr if a file with given filename 
 * does not exist. If -m N switch is provided, the maximum number of
 * allowed moves is set to N (1000 if omitted). If -c switch is provided,
 * the search uses a "cache" to improve the search time. If -f switch is
 * provided, the search forces "safe" moves to improve the total search time.
 * (-f switch may increase the size of the sequence). If -v switch is provided,
 * the search enters a verbose mode.
 * @param argc: number of the command line arguments
 * @param argv: array of strings that represent command line arguments
 * @return 0 if the program is successfully executed
 * */
int main(int argc, char* argv[]){
  FILE* inFile = stdin;
  char* infileName;
  int maxSearch = 1000;
  bool vformat = 0, cached = 0, forced = 0;
  
  for(int i=1; i<argc; i++){
    if(!strcmp(argv[i], "-m")){
      if(i+1 == argc)
        return 0 * fprintf(stderr, "Expecting a number after -m switch!\n");
      maxSearch = (int)atol(argv[++i]);
    }
    else if(!strcmp(argv[i], "-c"))
      cached = 1;
    else if(!strcmp(argv[i], "-f"))
      forced = 1;
    else if(!strcmp(argv[i], "-v"))
      vformat = 1;
    else if(argv[i][0] != '-')
      inFile = fopen(argv[i], "r"), infileName = argv[i];
  }
  
  if(!inFile)
    return 0 * fprintf(stderr, "Input is invalid: File %s does not exist!\n", infileName);
  
  int turns, limit, lcount;
  Card tableau[COLUMN][MAX_CARD], waste[MAX_STOCK], stock[MAX_STOCK], found[MAX_FOUND][MAX_CARD];
  int tSize[COLUMN], cSize[COLUMN], wasteSize, stockSize, fSize[MAX_FOUND];
  
  if(!part1(inFile, tableau, tSize, cSize, waste, &wasteSize, stock, &stockSize, found, fSize, &turns, &limit, &lcount))
    return 0;
  
  gamestate G = gamestate(tableau, waste, stock, found, tSize, cSize, wasteSize, stockSize, fSize, turns, limit);
  
  vector<string> ans;
  bool isFound = part3(G, ans, maxSearch, cached, forced, vformat);
  
  if(isFound){
    printf("# Game is winnable in %d moves:\n", (int)ans.size());
    reverse(ans.begin(), ans.end());
    for(string i : ans)
      cout << i << "\n";
  }
  else
    printf("# Game is not winnable within %d moves\n", maxSearch);
  
  return 0;
}
