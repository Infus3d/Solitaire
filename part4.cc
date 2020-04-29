#include "iostream"

#include "ncurses.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "locale.h"

#include "part1.h"
#include "part2.h"
#include "part3.hh"

// Useful constants
const char ranks[] = "_A23456789TJQK";
const char suits[] = "cdhs";

#define SPADES "\u2660"
#define HEARTS "\u2665"
#define DIAMON "\u2666"
#define CLUBS  "\u2663"

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
gamestate generate_from_seed(unsigned rSeed, int turns, int limit){
  Card deck[52];
  unsigned counter = 0;
  
  for(int i=0; i<MAX_FOUND; i++){
    for(int j=1; j<(int)strlen(ranks); j++){
      deck[counter].suit = suits[i];
      deck[counter].rank = ranks[j];
      deck[counter].isuit = i;
      deck[counter].irank = j;
      deck[counter].color = getColor(suits[i]);
      counter++;
    }
  }
  
  srand(rSeed);
  for(int i=0; i<52; i++){
    int j = rand() % (52-i) + i;
    if(i != j){
      Card temp = deck[i];
      deck[i] = deck[j];
      deck[j] = temp;
    }
  }
  
  counter = 0;
  gamestate G;
  for(int i=0; i<COLUMN; i++){
    G.cSize[i] = i;
    G.tSize[i] = i+1;
    for(int j=0; j<G.tSize[i]; j++)
      G.tableau[i][j] = deck[counter++];
  }
  
  G.wasteSize = 0;
  for(int i=0; i<MAX_FOUND; i++){
    G.fSize[i] = 1;
    G.found[i][0].color = getColor(suits[i]);
    G.found[i][0].rank = '_';
    G.found[i][0].irank = 0;
    G.found[i][0].suit = suits[i];
    G.found[i][0].isuit = getSuit(suits[i]);
  }
  
  G.turns = turns; G.limit = limit;
  while(counter < 52)
    G.stock[G.stockSize++] = deck[counter++];
  
  return G;
}

/* Helper struct that keeps track of the window
 * starting positions, columns and rows
 * */
struct myWindow{
  WINDOW *win;
  int cols, rows;
  int y, x;
};

/* Helper function that initializes all needed pair of colors
 * */
void init_color_pairs(){
  init_pair(1, COLOR_BLACK, COLOR_WHITE); // Color for uncovered spades and clubs
  init_pair(2, COLOR_RED, COLOR_WHITE);   // Color for uncovered hearts and diamonds
  init_pair(3, COLOR_BLACK, COLOR_GREEN); // Border color for hotkeys window and empty foundations
  init_pair(4, COLOR_BLACK, COLOR_CYAN);  // Color of the covered cards
  init_pair(5, COLOR_WHITE, COLOR_MAGENTA); // Color of the hotkeys
  init_pair(6, COLOR_WHITE, COLOR_BLACK); // Color for 'cleaning'
  init_pair(7, COLOR_RED, COLOR_BLACK); // Color for 'invalid move'
  init_pair(8, COLOR_GREEN, COLOR_BLACK); // Color for 'valid move'
  init_pair(9, COLOR_YELLOW, COLOR_BLACK); // Color for 'ok move'
  init_pair(10, COLOR_BLACK, COLOR_YELLOW); // Border color for hotkeys window
}

/* Helper function that takes in a hotkeys window and prompts the user
 * on that window for an outfile name.
 * @param hot: given hotkeys window
 * @return the outfile to write the current game state to (in exchange format)
 * */
FILE* prompt_for_outFile(myWindow &hot){
  wattron(hot.win, COLOR_PAIR(10));
  mvwprintw(hot.win, hot.rows-1, 1, "Filename: "); 
  wrefresh(hot.win);
  
  curs_set(2); echo();
  char str[27]; str[26] = 0;
  wgetstr(hot.win, str);
  wattroff(hot.win, COLOR_PAIR(10));
  curs_set(0); noecho();
  
  FILE *outFile = fopen(str, "w");
  return outFile;
}

/* Simply writes the given game state onto the given out file in exchange
 * format (Discribed in part2/part3).
 * @param outFile: given outfile to write on
 * @param G: given gamestate to "save"
 * */
void write_to_outFile(FILE *outFile, gamestate &G){
/************************ Exchange format ***************************/
  fprintf(outFile, "RULES:\nturn %d\n", G.turns);
  if(G.limit == 10) fprintf(outFile, "unlimited\n");
  else  fprintf(outFile, "limit %d\n", G.limit);
  fprintf(outFile, "FOUNDATIONS:\n");
  for(int i=0; i<MAX_FOUND; i++){
    fprintf(outFile, "%c%c", G.found[i][G.fSize[i]-1].rank, G.found[i][G.fSize[i]-1].suit);
    if(i+1 < MAX_FOUND)
      fprintf(outFile, " ");
  }
  fprintf(outFile, "\n");
  fprintf(outFile, "TABLEAU:\n");
  for(int i=COLUMN-1; i>=0; i--){
    if(!G.cSize[i]) fprintf(outFile, "| ");
    for(int j=0; j<G.tSize[i]; j++){
      fprintf(outFile, "%c%c ", G.tableau[i][j].rank, G.tableau[i][j].suit);
      if(G.cSize[i] == j+1)
        fprintf(outFile, "| ");
    }
    fprintf(outFile, "\n");
  }
  fprintf(outFile, "STOCK:\n");
  for(int i=0; i<G.wasteSize; i++)  fprintf(outFile, "%c%c ", G.waste[i].rank, G.waste[i].suit);
  fprintf(outFile, "| ");
  for(int i=G.stockSize-1; i>=0; i--) fprintf(outFile, "%c%c ", G.stock[i].rank, G.stock[i].suit);
  fprintf(outFile, "\nMOVES:\n");
  fclose(outFile);
}

/* Helper function that takes in a character indicating a suit ('c','h','d', 's')
 * and returns the unicode equivalent symbol of the suit.
 * @param suit: given suit to get the Unicode character of
 * @return the Unicode character of the suit (UTF-8, probs)
 * */
wchar_t cardUnicode (char suit) {
	if ('c'==suit) return (wchar_t)L'\u2663';
	else if ('s'==suit) return L'\u2660';
	else if ('h'==suit) return L'\u2665';
	else if ('d'==suit) return L'\u2666';
	else {
		endwin();
		fprintf(stderr, "Programmer error caught in cardUnicode - illegal card suit");
		exit(1);
	}
}

/* One of the three (3) main windows in this game is table. It simply stores
 * the tableau and is called everytime if any action occurs. The function first
 * clears its every cell and then writes the updated gamestate as tableau.
 * @param T: given table window
 * @param G: given gamestate
 * */
void print_table(myWindow &T, gamestate &G){
  wattron(T.win, COLOR_PAIR(6));
  for(int i=0; i<T.rows; i++)
    for(int j=0; j<T.cols; j++)
      mvwaddch(T.win, i, j, ' ');
  wattroff(T.win, COLOR_PAIR(6));
  wrefresh(T.win);
  
  wmove(T.win, 0, 0);
  char cn = '1';
  for(int i=0; i<T.cols; i++){
    if((i-3) >= 0 && (i-3) % 6 == 0){
      wattron(T.win, COLOR_PAIR(5) | A_BOLD);
      mvwaddch(T.win, 0, i, cn++);
      wattroff(T.win, COLOR_PAIR(5) | A_BOLD);
    }
  }
  
  int dx[] = {2, 8, 14, 20, 26, 32, 38};
  int dy[] = {1, 1, 1, 1, 1, 1, 1};
  
  wattron(T.win, COLOR_PAIR(4));
  for(int i=0; i<COLUMN; i++)
    if(G.cSize[i] > 0){
      for(int u=0; u<G.cSize[i]; u++)
        for(int v=0; v<4; v++)
          mvwaddch(T.win, u+dy[i], v+dx[i], '#');
    }
  wattroff(T.win, COLOR_PAIR(4));
  
  wattron(T.win, COLOR_PAIR(1));
  for(int i=0; i<COLUMN; i++)
    if(G.tSize[i] > 0){
      for(int u=G.cSize[i]; u<G.tSize[i]+3; u++)
        for(int v=0; v<4; v++)
          mvwaddch(T.win, u+dy[i], v+dx[i], ' ');
    }
  wattroff(T.win, COLOR_PAIR(1));
  
  for(int i=0; i<COLUMN; i++){
    if(G.tSize[i] > 0){
      for(int u=G.cSize[i]; u<G.tSize[i]; u++){
        Card cur = G.tableau[i][u];
        int colP = cur.color < 0 ? 1 : 2;
        
        wattron(T.win, COLOR_PAIR(colP));
        mvwprintw(T.win, u+dy[i], dx[i], "%c%lc", cur.rank, cardUnicode(cur.suit));
        if(u == G.tSize[i]-1)
          mvwprintw(T.win, u+dy[i]+3, dx[i]+1, "%lc %c", cardUnicode(cur.suit), cur.rank);
        wattroff(T.win, COLOR_PAIR(colP));
      }
    }
  }
  
  wrefresh(T.win);
}

/* Simple min function of two integers
 * */
inline int min(int x, int y) { return x < y ? y : x; }

/* One of the three (3) main windows in this game is misc. Misc is used
 * to display the Foundations and waste/stock of the given game state.
 * The function first clears everything before proceeding with updates.
 * @param M: given misc window
 * @param G: given game state to display
 * */
void print_misc(myWindow &M, gamestate &G){
  wattron(M.win, COLOR_PAIR(6));
  for(int i=0; i<M.rows; i++)
    for(int j=0; j<M.cols; j++)
      mvwaddch(M.win, i, j, ' ');
  wattroff(M.win, COLOR_PAIR(6));
  wrefresh(M.win);
  
  char msg[] = "OUNDATIONS";
  wattron(M.win, COLOR_PAIR(5) | A_BOLD);
  mvwaddch(M.win, 0, (M.cols - (strlen(msg) + 1)) / 2, 'F'); 
  wattroff(M.win, COLOR_PAIR(5) | A_BOLD);
  wattron(M.win, COLOR_PAIR(6));
  wprintw(M.win, "%s", msg);
  wattroff(M.win, COLOR_PAIR(6));
  
  int dx[] = {5, 12, 19, 26};
  int dy[] = {2, 2, 2, 2};
  
  for(int i=0; i<MAX_FOUND; i++){
    int colP = G.fSize[i] > 1 ? (getColor(suits[i]) < 0 ? 1 : 2) : 3;
    wattron(M.win, COLOR_PAIR(colP));
    if(G.fSize[i] == 1){
      for(int u=0; u<4; u++)
        for(int v=0; v<5; v++)
          mvwprintw(M.win, u+dy[i], v+dx[i], "%lc", v == 4 ? ' ' : cardUnicode(suits[i]));
    }
    else{
      Card cur = G.found[i][G.fSize[i]-1];
      for(int u=0; u<4; u++)
        for(int v=0; v<4; v++){
          if(!u && !v)
            mvwprintw(M.win, u+dy[i], v+dx[i], "%c%lc", cur.rank, cardUnicode(cur.suit)), v++;
          else if(u+1 == 4 && v+3 == 4)
            mvwprintw(M.win, u+dy[i], v+dx[i], "%lc %c", cardUnicode(cur.suit), cur.rank), v++;
          else
            mvwprintw(M.win, u+dy[i], v+dx[i], " ");
        }
    }
    wattroff(M.win, COLOR_PAIR(colP));
  }
  
  char *amsg = 0;
  char msg1[] = "ASTE (2 resets remaining)";
  char msg2[] = "ASTE (unlimited resets)";
  if(G.limit < 10) amsg = msg1, amsg[6] = G.limit + '0';
  else amsg = msg2;
  wattron(M.win, COLOR_PAIR(5) | A_BOLD);
  mvwaddch(M.win, 7, (M.cols - (strlen(amsg) + 1)) / 2 + 1, 'W'); 
  wattroff(M.win, COLOR_PAIR(5) | A_BOLD);
  wattron(M.win, COLOR_PAIR(6));
  wprintw(M.win, "%s", amsg);
  wattroff(M.win, COLOR_PAIR(6));
  
  if(G.wasteSize > 0){
    Card top = G.waste[G.wasteSize-1];
    int colP = top.color < 0 ? 1 : 2;
    wattron(M.win, COLOR_PAIR(colP));
    for(int u=0; u<4; u++)
      for(int v=0-((min(G.wasteSize, 3)-1)*3); v<4; v++)
        mvwprintw(M.win, u+9, v+dx[1], " ");
    wattroff(M.win, COLOR_PAIR(colP));
    
    for(int i=max(G.wasteSize-3, 0); i<G.wasteSize; i++){
      Card cur = G.waste[i];
      colP = cur.color < 0 ? 1 : 2;
      wattron(M.win, COLOR_PAIR(colP));
      mvwprintw(M.win, 9, dx[1] - ((G.wasteSize - i - 1) * 3), "%c%lc", cur.rank, cardUnicode(cur.suit));
      if(i+1 == G.wasteSize)
        mvwprintw(M.win, 12, dx[1]+1, "%lc %c", cardUnicode(cur.suit), cur.rank);
      wattroff(M.win, COLOR_PAIR(colP));
    }
  }
  
  if(G.stockSize > 0){
    wattron(M.win, COLOR_PAIR(4));
    for(int i=0; i<4; i++)
      for(int j=0; j<4; j++)
        mvwprintw(M.win, i+9, dx[1]+12+j, "#");
    wattroff(M.win, COLOR_PAIR(4));
  }
  
  wattron(M.win, COLOR_PAIR(6));
  mvwprintw(M.win, 13, dx[1]+1, "%d", G.wasteSize);
  mvwprintw(M.win, 13, dx[1]+13, "%d", G.stockSize);
  wattroff(M.win, COLOR_PAIR(6));
  
  wrefresh(M.win);
}

/* One of the three (3) main windows in this game is hotkeys. The window
 * is used to display the instructions on how to play the game: the hotkeys,
 * the order of moves, etc. It also encapsulates the save to file text box
 * at the bottom of the window (it is displayed whenever user presses S to
 * save).
 * @param H: given hotkeys window
 * @param G: given game state to display
 * @param fileName: fileName and rSeed are needed to display the source of the
 * game (if it is a random game or file created)
 * @param rSeed: fileName and rSeed are needed to display the source of the game
 * (if it is a random game or file specified)
 * */
void print_hotkeys(myWindow &H, gamestate &G, char *fileName, unsigned rSeed){
  wattron(H.win, COLOR_PAIR(6));
  for(int i=0; i<H.rows; i++)
    for(int j=0; j<H.cols; j++)
      mvwaddch(H.win, i, j, ' ');
  wattroff(H.win, COLOR_PAIR(6));
  wrefresh(H.win);
  
  wattron(H.win, COLOR_PAIR(10));
  wborder(H.win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  
  char *asmg = 0;
  if(fileName){
    char str[101] = "File: ";
    sprintf(str, "File: %s", fileName);
    asmg = str;
  }
  else{
    char str[101];
    sprintf(str, "Random: seed %u", rSeed);
    asmg = str;
  }
  mvwprintw(H.win, 0, (H.cols - strlen(asmg)) / 2, "%s", asmg);
  wattroff(H.win, COLOR_PAIR(10));
  
  wattron(H.win, COLOR_PAIR(5) | A_BOLD);
  mvwprintw(H.win, 1, 2, "Q");    mvwprintw(H.win, 1, 17, "S");
  mvwprintw(H.win, 2, 2, "R");    
  mvwprintw(H.win, 3, 2, "N");    mvwprintw(H.win, 3, 17, "U");
  mvwprintw(H.win, 4, 2, ".");    mvwprintw(H.win, 4, 17, ">");
  wattroff(H.win, COLOR_PAIR(5) | A_BOLD); 
  
  wattron(H.win, COLOR_PAIR(6));
  mvwprintw(H.win, 1, 3, ": quit");       mvwprintw(H.win, 1, 18, ": save to file");
  mvwprintw(H.win, 2, 3, ": restart");
  mvwprintw(H.win, 3, 3, ": new game");   mvwprintw(H.win, 3, 18, ": undo move");   
  mvwprintw(H.win, 4, 3, ": next card");  mvwprintw(H.win, 4, 18, ": reset stock");
  mvwprintw(H.win, 5, 2, "To move, press the source and");
  mvwprintw(H.win, 6, 2, "then destination (one of W, F,");
  mvwprintw(H.win, 7, 2, "1, 2, 3, 4, 5, 6, 7)");
  wattroff(H.win, COLOR_PAIR(6));
  
  wrefresh(H.win);
}

/* This function simply calls its sub-functions, in order. First display
 * the table, then misc, hotkeys and status. The parameters are needed for
 * some of the sub-functions for varius reasons (refer to their documentation).
 * */
void print_to_stdscr(gamestate &G, myWindow &table, myWindow &misc, 
      myWindow &hotkeys, myWindow status, char *fileName, unsigned rSeed){
  print_table(table, G);
  print_misc(misc, G);
  print_hotkeys(hotkeys, G, fileName, rSeed);
  wrefresh(status.win);
}

/* Small window that appears at the bottom of the left half of the screen.
 * It is called a 'status' window because it is used to let user know about
 * many things that might help the user's interaction with the game. For ex:
 * it tells if the move the user just did was valid or invalid (it is not actually
 * done, if it is invalid).
 * @param S: the status window
 * @param w: an integer that is used in switch/case statement. Every different
 * value passed indicated different status.
 * @param A: the first character defining a move
 * @param B: the second character defingin a move, if applicable
 * */
void update_status(myWindow &S, int w, char A = 0, char B = 0){
  wattron(S.win, COLOR_PAIR(6));
  for(int i=0; i<S.cols; i++)
    mvwprintw(S.win, 0, i, " ");
  mvwprintw(S.win, 0, 0, "Status: ");
  wattroff(S.win, COLOR_PAIR(6));
  wrefresh(S.win);
  
  char move[5];
  if(A) move[0] = A, move[1] = 0;
  if(B) move[1] = '-', move[2] = '>', move[3] = B, move[4] = 0;
  
  int colP = (w == 3 || w == 6 || w == 7) ? 9 : (w == 0 || w == 2) ? 7 : (w == 1 || w == 5) ? 8 : 6;
  wattron(S.win, COLOR_PAIR(colP));
  switch(w){
    case 0:
      mvwprintw(S.win, 0, 8, "Invalid move %s", move);
      break;
    case 1:
      mvwprintw(S.win, 0, 8, "Move %s is valid", move);
      break;
    case 2:
      mvwprintw(S.win, 0, 8, "Maximum allowed move # is reached!");
      break;
    case 3:
      mvwprintw(S.win, 0, 8, "No more moves left to undo");
      break;
    case 4:
      mvwprintw(S.win, 0, 8, "None");
      break;
    case 5:
      mvwprintw(S.win, 0, 8, "Undo successful");
      break;
    case 6:
      mvwprintw(S.win, 0, 8, "Please enter upto 25 characters");
      break;
    case 7:
      mvwprintw(S.win, 0, 8, "Please enter the destination");
      break;
    case 8:
      wattroff(S.win, COLOR_PAIR(6));
      wattron(S.win, COLOR_PAIR(8) | A_BOLD | A_BLINK);
      mvwprintw(S.win, 0, 8, "Congrats! You Won!");
      wattroff(S.win, COLOR_PAIR(8) | A_BOLD | A_BLINK);
      break;
  }
  wattroff(S.win, COLOR_PAIR(colP));
  
  wrefresh(S.win);
}

/* Helper function that determines if the game is won in its given state.
 * @param G: given gamestate to check
 * @return true if all of the cards are in the FOUNDATIONS, false otherwise.
 * */
bool isWon(gamestate &G){
  return (G.fSize[0] + G.fSize[1] + G.fSize[2] + G.fSize[3] == 56);
}

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
void part4(gamestate H, unsigned _rSeed, int _turns, int _limit, char* fileName){
  setlocale(LC_ALL, "");
  gamestate G[1005];
  // gamestate *G = new gamestate[1005];
  G[0] = H;
  unsigned curN = 0, rSeed = _rSeed;
  myWindow table, misc, hotkeys, status;
  
  initscr(); cbreak();
  keypad(stdscr, TRUE);
  start_color(); noecho();
  // getmaxyx(stdscr, rows, cols);
  curs_set(0);
  
  init_color_pairs();
  
  table.rows = 24; table.cols = 45;
  table.y = 0; table.x = 0;
  table.win = newwin(table.rows, table.cols, table.y, table.x);
  
  misc.rows = 14; misc.cols = 35;
  misc.y = 0; misc.x = 45;
  misc.win = newwin(misc.rows, misc.cols, misc.y, misc.x);
  
  hotkeys.rows = 10; hotkeys.cols = 35;
  hotkeys.y = 15, hotkeys.x = 45;
  hotkeys.win = newwin(hotkeys.rows, hotkeys.cols, hotkeys.y, hotkeys.x);
  
  status.rows = 1, status.cols = 45;
  status.y = 24; status.x = 0;
  status.win = newwin(status.rows, status.cols, status.y, status.x);
  
  update_status(status, 4);   // 4 for none
  print_to_stdscr(G[curN], table, misc, hotkeys, status, fileName, rSeed);
  refresh();
  
  while(true){
    int ch = getch();
    if(ch == 'q' || ch == 'Q')
      break;
    else if(ch == 'r' || ch == 'R'){
      curN = 0;
      update_status(status, 4);
    }
    else if(ch == 'u' || ch == 'U'){
      if(curN) curN--, update_status(status, 5);    // 5 for valid undo
      else update_status(status, 3);    // 3 for no moves left to undo
    }
    else if(ch == 'n' || ch == 'N'){
      srand((unsigned)time(NULL));
      rSeed = rand();
      G[curN = 0] = generate_from_seed(rSeed, 1, 10);
    }
    else if(ch == 's' || ch == 'S'){
      update_status(status, 6);
      FILE *outFile = prompt_for_outFile(hotkeys);
      write_to_outFile(outFile, G[curN]);
    }
    else if(curN == 1000)
      update_status(status, 2, 'A', 'A');  // 2 for no more moves
    else{
      G[curN+1] = G[curN];
      if(ch == 'w' || ch == 'W' || ('1' <= ch && ch <= '7')){
        char A = (char) ch;
        update_status(status, 7);
        char B = (char) getch();
        if((B == 'f' || B == 'F' || ('1' <= B && B <= '7')) && isValid(G[curN+1], A, B))
          update_status(status, 1, A, B), curN++;   // 1 for valid move
        else
          update_status(status, 0, A, B);   // 0 for invalid move
      }
      else if(ch == '.'){
        char A = (char) ch;
        if(isValid(G[curN+1], A, A))
          update_status(status, 1, A), curN++;
        else
          update_status(status, 0, A);
      }
      else if(ch == '>'){
        char A = (char) 'r';
        if(isValid(G[curN+1], A, A))
          update_status(status, 1, '>'), curN++;
        else
          update_status(status, 0, '>');
      }
      else
        update_status(status, 0, (char) ch);
    }
    
    if(isWon(G[curN]))  update_status(status, 8);
    print_to_stdscr(G[curN], table, misc, hotkeys, status, fileName, rSeed);
  }
  
  endwin();
  // delete[] G;
}
