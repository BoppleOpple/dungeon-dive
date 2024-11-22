#include "roomManip.h"
#include "stringManip.h"
#include "dungeon.h"
#include "list.h"
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 32

/*
 * takes a line of input and returns a list of commands
**/
LIST parseCommand(char *str) {
	// could use malloc later to save memory if i wanted
	LIST arguments = listCreate();

	// to find arguments, keep track of the last space found eg.
	//        ^      ^   ^
	// lastSpace     i1  i2
	//
	// once i is at a space, the string from lastSpace+1 to i-1 (inclusive) is a word
	// (if there are no characters in that range, it isn't a word)
	int lastSpace = -1;
	for (int i = 0; i < strlen(str); i++) {
		// if the character at i is a space, we are at the end of a word
		if (isspace(*(str + i))) {
			// if two spaces are in a row, dont bother saving it
			if (lastSpace < i-1)
				listAppendItem(&arguments, str_cut(str, lastSpace + 1, i - 1));
			// update lastSpace
			lastSpace = i;
		}
	}

	return arguments;
}

int main(int argc, char *argv[]) {
	// declare variables
	char lineBuffer[LINE_BUFFER_SIZE];
	LIST roomList;
	LIST argumentList;
	ROOM *dungeon = NULL;
	ROOM *location = NULL;
	int dungeonWidth = 0;
	int dungeonHeight = 0;

	// seed rng
	srand(time(0));

	// yell if a file isn't passed
	if (argc < 2) {
		printf("please pass a file name\n");
		return 1;
	}

	// read from the file of possible rooms
	roomList = readRoomFile(argv[1]);
	if (roomList.size == 0) {
		printf("No rooms loaded!\nexiting...");
		return 1;
	}

	// prompt for the dungeon size
	printf("enter the dungeon width:\n");
	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);
	dungeonWidth = atoi(lineBuffer);

	printf("enter the dungeon height:\n");
	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);
	dungeonHeight = atoi(lineBuffer);

	printf("creating %ix%i dungeon...\n", dungeonWidth, dungeonHeight);

	// create the dungeon and set the PC's location
	dungeon = createDungeon(&roomList, &dungeonWidth, &dungeonHeight);
	location = dungeon;

	printDungeon(dungeon, &dungeonWidth, &dungeonHeight);

	// game loop
	int loop = 1;
	while (loop) {
		// print the current room description and options
		printf("%s\n", location->description);

		printf("These are the directions you can move: ");
		if (location->north) printf("N ");
		if (location->south) printf("S ");
		if (location->east)  printf("E ");
		if (location->west)  printf("W ");
		printf("\n");

		// if an incorrect command is entered, just ask for input again
		int enteringCommand = 1;
		while(enteringCommand) {
			// ask for input and clear the screen afterwards for readability
			printf("\nWhat will you do?\n");
			fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);

			printf("\033[2J");

			argumentList = parseCommand(lineBuffer);

			// if nothing was inputted, ask again
			if (argumentList.size == 0)
				continue;

			// check against each command
			if (strcmp(listGetElement(&argumentList, 0), "move") == 0) {
				ROOM *destination = location;

				// set the location depending on the next argument
				switch (*((char*) listGetElement(&argumentList, 1))) {
					case 'n':
					case 'N':
						destination = location->north;
						break;
					case 's':
					case 'S':
						destination = location->south;
						break;
					case 'e':
					case 'E':
						destination = location->east;
						break;
					case 'w':
					case 'W':
						destination = location->west;
						break;
					default:
						printf("Whatever dimension you attempted to move in, your poor 2-dimensional self was incapable.\nYou must choose either n, s, e, or w.\n");
				}
				// if a direction was chosen
				if (destination != location) {
					// and the direction is valid
					if (destination) {
						// move and continue to the next step
						location = destination;
						enteringCommand = 0;
					} else {
						printf("There's a wall that way, and no you may not fight it.\n");
					}
				}

			} else if (strcmp(listGetElement(&argumentList, 0), "fireball") == 0) {
				// exit and delete the dungeon
				loop = 0;
				enteringCommand = 0;
			} else {
				// if the input is not recognized, let the user know
				printf("I've never heard of a \"%s\" in my life.\n", (char*) listGetElement(&argumentList, 0));
			}

			// free the argument list
			while (argumentList.size > 0) {
				char *argument = listPop(&argumentList, argumentList.size-1);
				free(argument);
			}
			listClear(&argumentList);
		}

		printf("\n\n");
	}

	// once the game loop is over, delete the dungeon
	deleteDungeon(dungeon);

	// and free the rooms in the room list
	while (roomList.size > 0) {
		ROOM *currentRoom = listPop(&roomList, roomList.size-1);
		free(currentRoom->roomCode);
		free(currentRoom->description);
		free(currentRoom->name);
		free(currentRoom);
	}

	listClear(&roomList);

	return 0;
}