CC = g++
CFLAGS = -Wall -g -std=c++17


main: main.o combatTracker.o combatant.o initialize.o entryValidation.o
	$(CC) $(CFLAGS) -o initiativeTracker main.o combatTracker.o combatant.o initialize.o entryValidation.o -lstdc++fs

main.o: main.cpp combatant.h combatTracker.h initialize.h entryValidation.h
	$(CC) $(CFLAGS) -c main.cpp

combatTracker.o: combatTracker.h combatTracker.cpp

initialize.o: initialize.h initialize.cpp

combatant.o: combatant.h combatant.cpp

entryValidation.o: entryValidation.h entryValidation.cpp

clean:
	rm *.o