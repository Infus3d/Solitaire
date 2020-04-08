all: check advance winnable

winnable: winnableMain.o part3.o part2.o part1.o
	g++ -Wall -o winnable winnableMain.o part3.o part2.o part1.o

winnableMain.o: winnableMain.cc part3.hh part2.h part1.h
	g++ -Wall -c winnableMain.cc

part3.o: part3.cc part3.hh part2.h part1.h
	g++ -Wall -c part3.cc

advance: advanceMain.o part2.o part1.o
	g++ -Wall -o advance advanceMain.o part2.o part1.o

advanceMain.o: advanceMain.c part2.h part1.h
	g++ -Wall -c advanceMain.c

part2.o: part2.c part2.h part1.h
	g++ -Wall -c part2.c
	
check: checkMain.o part1.o
	g++ -Wall -o check checkMain.o part1.o

checkMain.o: checkMain.c part1.h
	g++ -Wall -c checkMain.c

part1.o: part1.c part1.h
	g++ -Wall -c part1.c
