check: main.o part1.o
	gcc -Wall -o check main.o part1.o

main.o: main.c part1.h
	gcc -Wall -c main.c

part1.o: part1.c part1.h
	gcc -Wall -c part1.c
