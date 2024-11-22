#include "dungeon.h"
#include <stdio.h>
#include <stdlib.h>

ROOM *createDungeon(LIST *roomList, int *width, int *height) {
	// break if the dungeon doesn't have any rooms
	if (*width * *height <= 0)
		return NULL;

	// declare variables
	ROOM *dungeon = NULL;
	ROOM **currentRow = &dungeon;
	ROOM *upperRoom = NULL;
	ROOM *currentRoom = NULL;

	for (int i = 0; i < *height; i++) {
		// create the first room of the row
		*currentRow = roomCreate(listGetElement(roomList, rand() % roomList->size));
		currentRoom = *currentRow;

		// make the rest of the rooms in the row
		for (int j = 0; j < *width - 1; j++) {
			// make the next room and link it horizontally
			currentRoom->east = roomCreate(listGetElement(roomList, rand() % roomList->size));
			currentRoom->east->west = currentRoom;

			// and link with the room in the previous row if it exists
			if (upperRoom) {
				upperRoom->south = currentRoom;
				currentRoom->north = upperRoom;
				upperRoom = upperRoom->east;
			}

			// move to the next
			currentRoom = currentRoom->east;
		}

		// link the final room (no fenceposting here)
		if (upperRoom) {
			upperRoom->south = currentRoom;
			currentRoom->north = upperRoom;
			upperRoom = upperRoom->east;
		}

		// move to a new row
		upperRoom = *currentRow;
		currentRow = &(*currentRow)->south;
	}

	return dungeon;
}

void dungeonDFT(ROOM *dungeon, void (*fn)(ROOM*)) {
	// declare vars
	LIST frontier = listCreate();
	LIST seen = listCreate();
	ROOM *currentRoom = NULL;
	ROOM *neighbors[4] = {NULL, NULL, NULL, NULL};

	// start with theroot on the frontier and seen lists
	listAppendItem(&frontier, dungeon);
	listAppendItem(&seen, dungeon);

	while (frontier.size > 0) {
		// pick the last room in the list (dft)
		currentRoom = listPop(&frontier, frontier.size-1);

		neighbors[0] = currentRoom->north;
		neighbors[1] = currentRoom->south;
		neighbors[2] = currentRoom->east;
		neighbors[3] = currentRoom->west;

		// iterate through the neighbors and add them to the frontier if they are new
		for (int i = 0; i < 4; i++) {
			if (*(neighbors + i) != NULL && !listIncludesItem(&seen, *(neighbors + i))) {
				listAppendItem(&frontier, *(neighbors + i));
				listAppendItem(&seen, *(neighbors + i));
			}
		}
		// call fn on the popped room (postorder)
		fn((ROOM*) currentRoom);
	}
	
	// free the lists, but not their rooms
	listClear(&frontier);
	listClear(&seen);
}

void printDungeon(ROOM *dungeonCorner, int *width, int *height) {
	// declare vars
	ROOM *rowHead = dungeonCorner;
	ROOM *room = rowHead;

	// print row by row
	for (int i = 0; i < *height; i++) {

		// print the entire row's rooms
		room = rowHead;
		for (int j = 0; j < *width; j++) {
			printf("%s %c ", room->roomCode, (room->east) ? '=' : ' ');
			room = room->east;
		}
		printf("\n");

		// print the vertical connections
		room = rowHead;
		for (int j = 0; j < *width; j++) {
			printf("%s   ", (room->south) ? "||" : "  ");
			room = room->east;
		}
		printf("\n");

		// move the row
		rowHead = rowHead->south;
	}
}

// calls free on a room
void freeAsRoom(ROOM *room) {
	free(room);
}

void deleteDungeon(ROOM *dungeon) {
	// frees each room using dft
	dungeonDFT(dungeon, &freeAsRoom);
}
