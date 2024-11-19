#include "roomManip.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 32

/*
 * takes a room pointer and prints the room code and name
**/
void printRoom(ROOM *room) {
	printf("Room %s: %s\n\n", room->roomCode, room->name);
}

/*
 * creates a 1d dungeon by extending a linked list using the rooms in roomArray,
 * and returns a pointer to the first room of the dungeon.
**/
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

/*
 * iterates through and prints the contents of each room in the dungeon
**/
void printDungeon(ROOM *dungeon) {
	for (ROOM *next = dungeon; next != NULL; next = next->east)
		printRoom(next);
}

/*
 * frees each room of a dungeon, which assumes the content of each room will be freed in the roomArray.
 * as a result, this function is really simple since the pointers in each struct are all duplicates
**/
void deleteDungeon(ROOM *dungeon) {
	// LIST frontier = listCreate();
}

// I started writing this function (and refactoring like half the code) about 4 hours ago before realizing
// that you only wanted a 1d list
// void printDungeon(ROOM *dungeon) {
// 	ROOM_LIST printed = roomListCreate();
// 	ROOM_LIST frontier = roomListCreate();

// 	roomListAppendRoom(&frontier, dungeon);

// 	while (frontier.size > 0) {
// 		printRoom(frontier.array);

// 		if (frontier.array->north)
// 			if (!roomListIncludesCode(&printed, frontier.array->north->roomCode))
// 				roomListAppendRoom(&frontier, frontier.array->north);
// 		if (frontier.array->south)
// 			if (!roomListIncludesCode(&printed, frontier.array->south->roomCode))
// 				roomListAppendRoom(&frontier, frontier.array->south);
// 		if (frontier.array->east)
// 			if (!roomListIncludesCode(&printed, frontier.array->east->roomCode))
// 				roomListAppendRoom(&frontier, frontier.array->east);
// 		if (frontier.array->west)
// 			if (!roomListIncludesCode(&printed, frontier.array->west->roomCode))
// 				roomListAppendRoom(&frontier, frontier.array->west);
		
// 		roomListAppendRoom(&printed, frontier.array);
		
// 		roomListDelete(&frontier, 0); // really bad delete function for now
// 	}
// }

int main(int argc, char *argv[]) {
	char lineBuffer[LINE_BUFFER_SIZE];
	LIST roomList;
	ROOM *dungeon;

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

	printf("enter the number of rooms:\n");

	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);

	printf("creating dungeon with %i rooms...\n", atoi(lineBuffer));

	dungeon = createDungeon(&roomList, atoi(lineBuffer));

	printDungeon(dungeon);

	deleteDungeon(dungeon);

	// now be a "good" programmer and dont leak your memory
	while (roomList.size > 0) {
		ROOM *currentRoom = listPop(&roomList, roomList.size-1);
		free(currentRoom->roomCode);
		free(currentRoom->description);
		free(currentRoom->name);
		free(currentRoom);
	}

	listFree(&roomList);

	return 0;
}