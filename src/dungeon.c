#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>

ROOM *createDungeon(LIST *roomList, int *width, int *height) {
	if (*width * *height <= 0)
		return NULL;
	ROOM *dungeon = NULL;
	ROOM **currentRow = &dungeon;
	ROOM *upperRoom = NULL;
	ROOM *currentRoom = NULL;

	for (int i = 0; i < *height; i++) {
		// create the first room of the row
		*currentRow = roomCreate(listGetElement(roomList, rand() % roomList->size));
		currentRoom = *currentRow;

		// make the rest of the rooms
		for (int j = 1; j < *width; j++) {
			currentRoom->east = roomCreate(listGetElement(roomList, rand() % roomList->size));
			currentRoom->east->west = currentRoom;

			if (upperRoom) {
				upperRoom->south = currentRoom;
				currentRoom->north = upperRoom;
				upperRoom = upperRoom->east;
			}

			currentRoom = currentRoom->east;
		}

		if (upperRoom) {
			upperRoom->south = currentRoom;
			currentRoom->north = upperRoom;
			upperRoom = upperRoom->east;
		}

		upperRoom = *currentRow;
		currentRow = &(*currentRow)->south;
	}

	return dungeon;
}

void dungeonDFT(ROOM *dungeon, void (*fn)(ROOM*)) {
	LIST frontier = listCreate();
	LIST seen = listCreate();
	ROOM *currentRoom;
	ROOM *neighbors[4] = {NULL, NULL, NULL, NULL};

	listAppendItem(&frontier, dungeon);
	listAppendItem(&seen, dungeon);

	while (frontier.size > 0) {
		currentRoom = listPop(&frontier, frontier.size-1);

		neighbors[0] = currentRoom->north;
		neighbors[1] = currentRoom->south;
		neighbors[2] = currentRoom->east;
		neighbors[3] = currentRoom->west;

		for (int i = 0; i < 4; i++) {
			if (*(neighbors + i) != NULL && !listIncludesItem(&seen, *(neighbors + i))) {
				listAppendItem(&frontier, *(neighbors + i));
				listAppendItem(&seen, *(neighbors + i));
			}
		}

		fn((ROOM*) currentRoom);
	}
	
	listClear(&frontier);
	listClear(&seen);
}

void printRoom(ROOM *room) {
	printf("Room %s: %s\n\n", room->roomCode, room->name);
}

void printDungeon(ROOM *dungeonCorner, int *width, int *height) {
	ROOM *rowHead = dungeonCorner;

	for (int i = 0; i < *height; i++) {
		ROOM *room = rowHead;
		for (int j = 0; j < *width; j++) {
			printf("%s %c ", room->roomCode, (room->east) ? '=' : ' ');
			room = room->east;
		}
		printf("\n");

		room = rowHead;
		for (int j = 0; j < *width; j++) {
			printf("%s   ", (room->south) ? "||" : "  ");
			room = room->east;
		}
		printf("\n");

		rowHead = rowHead->south;
	}
}

void freeAsRoom(ROOM *room) {
	free(room);
}

void deleteDungeon(ROOM *dungeon) {
	dungeonDFT(dungeon, &freeAsRoom);
}
