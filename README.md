# coms327project1
### Part 1
Part1 of the project reads game details from the given file (_stdin_ if not specified) and parses it for validity. At the end, the program either terminates after printing out the first error message or prints to stdout the number of covered, in-stock and in-waste cards in the game if the program runs smoothly (without any errors).  
Part1 was implemented **solely by me** (no partner) and below are the features I implemented from the pdf:
* READMEfile, that describes implemented features
* DEVELOPERSfile, that gives an overview of your implementation, including a breakdownof source files and functions, and who authored each function.
* WorkingMakefile.  Typing “make” builds the check executable.
* Reads fromstdinif no filename argument
* Reads from filename passed as argument
* Normal output tostdout
* Correctly formatted output on valid inputs
* Reports correct number of covered cards on valid inputs
* Reports correct number of stock cards on valid inputs
* Reports correct number of waste cards on valid inputs
* No error messages on valid inputs
* Error messages to stderr
* Formatting error messages include line numbers
* Appropriate error messages for formatting errors
* Checks that the tableau piles are legal
* Checks for duplicate cards
* Checks for missing cards

Additional features:
* Checks for the validity of the piles of tableau columns by observing if there is any uncovered cards after non-zero number of covered cards
* Store the data of the given state of the game for future use (tableau, stock, waste and foundations)


### Part 2
Part2 of the project reads game details from the provided file (_stdin_ if not specified) and parses it for validity interms of both format validity and validity as a game-move. If the format is illegal, writes to _stderr_ and indicates the offensive line. If the format is valid but the move can not be processed for that instant of the game state then outputs to _stdout_ the move number of the invalid move and writes to the provided output file the game state that was valid so far (_stdout_ if not specified with _-o_ switch). If there is neither formatting error nor invalid input, total number of processed moves is written to _stdout_ and the final game state is written to wither specified file or _stdout_. With the help of _-m N_ switch, the user can control the number of moves to process.  
Part2 was implemented **solely by me** (no partner) and below are the features I implemented from the pdf:
* READMEfile, that describes implemented features
* DEVELOPERSfile, that gives an overview of your implementation, including a breakdownof source files and functions, and who authored each function.
* WorkingMakefile.  Typing “make” builds executablescheckandadvance.
* Reads from _stdin_ if no filename argument
* Reads from filename passed as argument
* Summary output to _stdout_
* Output is in human-readable format
* Correctly processes moves
* _-m_ switch
* Appropriate error messages for formatting errors
* Catches invalid moves
* Stress tests
* Works for _turn 3_
* Works for _limit R_
* _-x_ switch for exchange format output
* _-o_ switch
