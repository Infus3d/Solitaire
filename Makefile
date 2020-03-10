all: check advance

testsAdvance: advance
	./advance input0.txt

advance: advanceMain.o part2.o part1.o
	gcc -Wall -o advance advanceMain.o part2.o part1.o

advanceMain.o: advanceMain.c part2.h part1.h
	gcc -Wall -c advanceMain.c

part2.o: part2.c part2.h part1.h
	gcc -Wall -c part2.c

testsCheck: check
	#cat input0.txt | ./check
	#cat input1.txt | ./check
	#./check input0.txt	
	#./check input1.txt
	#./check input2.txt
	#./check input3.txt
	#./check Part1/badChain0.txt
	#./check Part1/badChain1.txt
	#./check Part1/badChain2.txt
	
check: checkMain.o part1.o
	gcc -Wall -o check checkMain.o part1.o

checkMain.o: checkMain.c part1.h
	gcc -Wall -c checkMain.c

part1.o: part1.c part1.h
	gcc -Wall -c part1.c
