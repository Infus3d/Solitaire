# coms327project1

This is a terminal based Solitaire game that anyone can play right from the comfort of their terminal. The project was done in 4 parts with each part explained in detail below. The entire project was developed using C/C++ only. To play the game, please clone, make and run the _game_ executable. 

### Part 1
Part1 of the project the reads game details from the given file (_stdin_ if not specified) and parses it for validity. At the end, the program either terminates after printing out the first error message or prints to stdout the number of covered, in-stock and in-waste cards in the game if the program runs smoothly (without any errors).  
The implemented features in Part1:
* **README** file, that describes implemented features
* **DEVELOPERS** file, that gives an overview of my implementation, including a breakdown of source files and functions, and who authored each function.
* Working **Makefile**.  Typing “make” builds the _check_ executable.
* Reads from _stdin_ if no filename argument
* Reads from filename passed as argument
* Normal output tostdout
* Correctly formatted output on valid inputs
* Reports correct number of covered cards on valid inputs
* Reports correct number of stock cards on valid inputs
* Reports correct number of waste cards on valid inputs
* No error messages on valid inputs
* Error messages to _stderr_
* Formatting error messages include line numbers
* Appropriate error messages for formatting errors
* Checks that the tableau piles are legal
* Checks for duplicate cards
* Checks for missing cards

Additional features:
* Checks for the validity of the piles of tableau columns by observing if there is any uncovered cards after non-zero number of covered cards
* Store the data of the given state of the game for future use (tableau, stock, waste and foundations)


### Part 2
Part2 of the project reads game details from the provided file (_stdin_ if not specified) and parses it for validity in terms of both format validity and validity as a game-move. If the format is illegal, writes to _stderr_ and indicates the offensive line. If the format is valid but the move can not be processed for that instant of the game state then outputs to _stdout_ the move number of the invalid move and writes to the provided output file the game state that was valid so far (_stdout_ if not specified with _-o_ switch). If there is neither formatting error nor invalid input, total number of processed moves is written to _stdout_ and the final game state is written to either specified file or _stdout_. With the help of _-m N_ switch, the user can control the number of moves to process.  
The implemented features in Part2:
* **README** file, that describes implemented features
* **DEVELOPERS** file, that gives an overview of my implementation, including a breakdown of source files and functions, and who authored each function.
* Working **Makefile**.  Typing “make” builds executables _check_ and _advance_.
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


### Part3
Part3 of the project reads the initial game configuration from the provided file in the command line (_stdin_ if not specified) and returns a sequence of moves that leads to winning configuration. It finds the sequence by checking all possible moves within the provided range by using Depth First Search (DFS). The number of maximum allowed size of the sequence is controlled via _-m N_ switch and is set to _1000_ by default. The speed of the search can be controlled via _-c_ switch, in which case the search employs a cache to keep track of the previously encountered game states. If _-f_ switch is provided, the search will force a "safe" move wherever possible, even though doing so might lead to a longer sequence. The verbose mode can be enabled by _-v_ switch.  
The implemented features in Part3:
* **README** file, that describes implemented features
* **DEVELOPERS** file, that gives an overview of my implementation, including a breakdown of source files and functions, and who authored each function.
* Working **Makefile**.  Typing “make” builds executables _check_, _advance_ and _winnable_.
* Reads from _stdin_ if no filename argument
* Reads from filename passed as an argument
* Output written to _stdout_
* Output _"# Game is winnable/not winnable"_
* Move sequences formatted correctly
* _-m_ switch works
* Finds a winning sequence if one exists
* Works for _turn 3_
* Works for _limit R_
* _-f_ switch works
* _-c_ switch works


### Part4
Part4 of the project either reads the initial game configuration from the provided file or generates one randomly using the provided _seed_ via _-s seed_ switch. Afterwards, the program displays the game panels (Tableau, Foundation, Stock and Waste) on the terminal emulator with the help of ncurses library. The terminal is meant to be an easy-to-interact GUI for those who want to play simple Klondike Solitaire. Initial game state can be passed from a file by _-f fileName_ switch. If the _-s seed_ switch is chosen instead, then the user can control the number of turns (at a time) in the game by _-1_ and _-3_ switches. He/She can also control the limit of resets in the game with _-l limit_ switch where limit meets the criteria _limit < 10_. If the latter switch is omitted, the number of resets is assumed to be _unlimited_. The game is meant to be played in 25x80 linux terminal. The user might need to press an additional key after running the program to actually see the game.   
The implemented features in Part4:
* **README** file, that describes implemented features
* **DEVELOPERS** file, that gives an overview of my implementation, including a breakdown of source files and functions, and who authored each function.
* Working **Makefile**.
* My _check_ executable still works
* My _advance_ executable still works
* Tableau display
* Foundations display
* Waste top display
* Hotkeys display
* Game Play
* Quit exits cleanly
* _-f_ switch works
* _-s_ switch works
* Turn 3
* Limited stock resets
* Undo last move
* Undo several moves
