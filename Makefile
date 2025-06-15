all:
	gcc -Wall -std=c99 -o main *.c -lm
run:
	./main