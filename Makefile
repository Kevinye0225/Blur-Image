# CC = clang
# ARGS = -Wall


all: numbers blur1 blur2

numbers: numbers.c
	gcc numbers.c -o numbers -lpthread

blur1: blur1.c qdbmp.o
	clang -o blur1 blur1.c qdbmp.o

blur2: blur2.c qdbmp.o
	gcc blur2.c qdbmp.o -o blur2 -lpthread

qdbmp.o: qdbmp.c qdbmp.h
	clang -c qdbmp.c


clean:
	rm -rf main *.o

