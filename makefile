CC := gcc

final: encryption.o algoFunctions.o fileHandling.o matrixHandling.o memoryHandling.o help.o
		$(CC) encryption.o algoFunctions.o fileHandling.o matrixHandling.o memoryHandling.o help.o -o final

encryption.o: encryption.c encryption.h help.h
		$(CC) -c encryption.c

algoFunctions.o: algoFunctions.c algoFunctions.h help.h
		$(CC) -c algoFunctions.c

fileHandling.o: fileHandling.c fileHandling.h 
		$(CC) -c fileHandling.c

matrixHandling.o: matrixHandling.c matrixHandling.h 
		$(CC) -c matrixHandling.c

memoryHandling.o: memoryHandling.c memoryHandling.h
		$(CC) -c memoryHandling.c

help.o: help.c help.h
		$(CC) -c help.c

clean:
		del *.o final