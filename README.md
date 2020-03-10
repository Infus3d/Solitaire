# coms327project1
### Part1
This is the Part1 of the COMS327S20 Project. Part1 of the project requires to read game details from the given file (stdin if not specified) and parse it for validity. At the end, the program should either terminate after printing out the first error message or print to stdout the number of covered, in-stock and in-waste cards in the game if the program runs smoothly (without any errors).  
Part1 was implemented solely by me (no partner) and below are the features I implemented from the pdf:
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

###Part2
This is the Part2 of the COMS327S20 Project.
