# coms327project1
This is the part1 of the Project. Part1 of the project requires to read game details from the given file (stdin if not specified) and parse it for validity. At the end, the program should either terminate after printing out the first error message or print to stdout the number of covered, in-stock and in-waste cards in the game if the program runs smoothly (without any errors).
<br/>Part1 was implemented solely by me (no partner) and below are the features I implemented from the pdf: <br/>
	- READMEfile, that describes implemented features <br/>
	- DEVELOPERSfile, that gives an overview of your implementation, including a breakdownof source files and functions, and who authored each function. <br/>
	- WorkingMakefile.  Typing “make” builds the check executable. <br/>
	- Reads fromstdinif no filename argument <br/>
	- Reads from filename passed as argument <br/>
	- Normal output tostdout <br/>
	- Correctly formatted output on valid inputs <br/>
	- Reports correct number of covered cards on valid inputs <br/>
	- Reports correct number of stock cards on valid inputs <br/>
	- Reports correct number of waste cards on valid inputs <br/>
	- No error messages on valid inputs <br/>
	- Error messages to stderr <br/>
	- Formatting error messages include line numbers <br/>
	- Appropriate error messages for formatting errors <br/>
	- Checks that the tableau piles are legal <br/>
	- Checks for duplicate cards <br/>
	- Checks for missing cards <br/>
<br/>
Additional features: <br/>
	- Checks for the validity of the piles of tableau columns by observing if there is any uncovered cards after non-zero number of covered cards <br/>
	- Store the data of the given state of the game for future use (tableau, stock, waste and foundations) <br/>
