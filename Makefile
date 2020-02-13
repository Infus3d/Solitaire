tests: check
	cat input0.txt | ./check
	cat input1.txt | ./check
	./check input2.txt
	./check input3.txt
	./check input4.txt
	
check: main.o part1.o
	gcc -Wall -o check main.o part1.o

main.o: main.c part1.h
	gcc -Wall -c main.c

part1.o: part1.c part1.h
	gcc -Wall -c part1.c
