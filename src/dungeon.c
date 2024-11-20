#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>

ROOM *createDungeon(LIST *roomList, int dungeonSize) {
	// create the first room if any rooms should be created
	ROOM *dungeon = NULL;
	if (dungeonSize > 0)
		dungeon = roomCreate(listGetElement(roomList, rand() % roomList->size));
	ROOM *currentRoom = dungeon;

	// make the rest of the rooms
	for (int i = 1; i < dungeonSize; i++) {
		currentRoom->east = roomCreate(listGetElement(roomList, rand() % roomList->size));
		currentRoom->east->west = currentRoom;
		currentRoom = currentRoom->east;
	}

	return dungeon;
}

void dungeonDFT(ROOM *dungeon, void (*fn)(ROOM*)) {
	LIST frontier = listCreate();
	LIST visited = listCreate();
	ROOM *currentRoom;
	ROOM *neighbors[4] = {NULL, NULL, NULL, NULL};

	listAppendItem(&frontier, dungeon);

	while (frontier.size > 0) {
		currentRoom = listPop(&frontier, frontier.size-1);
		listAppendItem(&visited, currentRoom);

		neighbors[0] = currentRoom->north;
		neighbors[1] = currentRoom->south;
		neighbors[2] = currentRoom->east;
		neighbors[3] = currentRoom->west;

		for (int i = 0; i < 4; i++) {
			if (*(neighbors + i) != NULL && !listIncludesItem(&visited, *(neighbors + i)))
				listAppendItem(&frontier, *(neighbors + i));
		}

		fn((ROOM*) currentRoom);
	}
	
	listClear(&frontier);
	listClear(&visited);
}

void printRoom(ROOM *room) {
	printf("Room %s: %s\n\n", room->roomCode, room->name);
}

void printDungeon(ROOM *dungeon) {
	dungeonDFT(dungeon, &printRoom);
}

void freeAsRoom(ROOM *room) {
	free(room);
}

void deleteDungeon(ROOM *dungeon) {
	dungeonDFT(dungeon, &freeAsRoom);
}
