CC = g++
CFLAGS = -Wall -g


main: main.o combatTracker.o combative.o
	$(CC) $(CFLAGS) -o main main.o combatTracker.o combative.o

main.o: main.cpp enemy.h player.h combative.h combatTracker.h
	$(CC) $(CFLAGS) -c main.cpp

combatTracker.o: combatTracker.h combatTracker.cpp

combative.o: combative.h combative.cpp