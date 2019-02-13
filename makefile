main.o: main.c
	clang -g -c main.c

main: main.o
	clang -o main main.o
