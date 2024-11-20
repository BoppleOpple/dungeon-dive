#include "roomManip.h"
#include "stringManip.h"
#include "dungeon.h"
#include "list.h"
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

LIST parseCommand(char *str) {
	LIST arguments = listCreate();

	int lastSpace = -1;
	for (int i = 0; i < strlen(str); i++) {
		if (isspace(*(str + i))) {
			if (lastSpace < i-1)
				listAppendItem(&arguments, str_cut(str, lastSpace + 1, i - 1));
			lastSpace = i;
		}
	}

	return arguments;
}

#define LINE_BUFFER_SIZE 32
int main(int argc, char *argv[]) {
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

	roomList = readRoomFile(argv[1]);
	if (roomList.size == 0) {
		printf("No rooms loaded!\nexiting...");
		return 1;
	}

	// as proof all the rooms are saved, yell about it
	// printf("Saved %i rooms\n", roomCount);
	// for (int i = 0; i < roomCount; i++) {
	// 	printf("Room %s: %s\n\n", (roomArray+i)->roomCode, (roomArray+i)->name);
	// }

	printf("enter the dungeon width:\n");
	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);
	dungeonWidth = atoi(lineBuffer);

	printf("enter the dungeon height:\n");
	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);
	dungeonHeight = atoi(lineBuffer);

	printf("creating %ix%i dungeon...\n", dungeonWidth, dungeonHeight);

	dungeon = createDungeon(&roomList, &dungeonWidth, &dungeonHeight);
	location = dungeon;

	printDungeon(dungeon, &dungeonWidth, &dungeonHeight);

	int loop = 1;
	while (loop) {
		printf("%s\n", location->description);

		printf("These are the directions you can move: ");
		if (location->north) printf("N ");
		if (location->south) printf("S ");
		if (location->east)  printf("E ");
		if (location->west)  printf("W ");
		printf("\n");

		int enteringCommand = 1;
		while(enteringCommand) {
			printf("\nWhat will you do?\n");
			fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);

			argumentList = parseCommand(lineBuffer);
			if (argumentList.size == 0)
				continue;
			if (strcmp(listGetElement(&argumentList, 0), "move") == 0) {
				ROOM *destination = location;
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

				if (destination != location) {
					if (destination) {
						location = destination;
						enteringCommand = 0;
					} else {
						printf("There's a wall that way, and no you may not fight it.\n");
					}
				}
			} else if (strcmp(listGetElement(&argumentList, 0), "fireball") == 0) {
				loop = 0;
				enteringCommand = 0;
			} else {
				printf("I've never heard of a \"%s\" in my life.\n", (char*) listGetElement(&argumentList, 0));
			}

			while (argumentList.size > 0) {
				char *argument = listPop(&argumentList, argumentList.size-1);
				free(argument);
			}
			listClear(&argumentList);
		}

		printf("\n\n");
	}

	deleteDungeon(dungeon);

	// now be a "good" programmer and dont leak your memory
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