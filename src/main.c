#include "roomManip.h"
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
ROOM *createDungeon(ROOM *roomArray, int roomArraySize, int dungeonSize) {
	// create the first room if any rooms should be created
	ROOM *dungeon = dungeonSize <= 0 ? NULL : roomCreate(roomArray + rand() % roomArraySize);
	ROOM *currentRoom = dungeon;
	
	// make the rest of the rooms
	for (int i = 1; i < dungeonSize; i++) {
		currentRoom->east = roomCreate(roomArray + rand() % roomArraySize);
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
	for (ROOM *next = dungeon; next->east != NULL; next = next->east) {
		free(next->west);

		// if its the last room, free next and exit
		if (next->east == NULL) {
			free(next);
			break;
		}
	}
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
	int roomCount = 0;
	char lineBuffer[LINE_BUFFER_SIZE];
	ROOM *roomArray;
	ROOM *dungeon;

	// seed rng
	srand(time(0));

	// yell if a file isn't passed
	if (argc < 2) {
		printf("please pass a file name\n");
		return 1;
	}

	roomArray = readRoomFile(argv[1], &roomCount);
	if (roomArray == NULL) return 1;

	// as proof all the rooms are saved, yell about it
	// printf("Saved %i rooms\n", roomCount);
	// for (int i = 0; i < roomCount; i++) {
	// 	printf("Room %s: %s\n\n", (roomArray+i)->roomCode, (roomArray+i)->name);
	// }

	printf("enter the number of rooms:\n");

	fgets(lineBuffer, LINE_BUFFER_SIZE, stdin);

	printf("creating dungeon with %i rooms...\n", atoi(lineBuffer));

	dungeon = createDungeon(roomArray, roomCount, atoi(lineBuffer));

	printDungeon(dungeon);

	deleteDungeon(dungeon);

	// now be a "good" programmer and dont leak your memory
	for (int i = 0; i < roomCount; i++) {
		// also I know this is a horrible idea 99/100 times but I wanted to try void pointers for this.
		// if I'm using c I might as well have fun with it :P (this is why good was in quotes)
		void **field = (void **)(roomArray+i);

		// since every field is a pointer, I can just iterate through each field bc theyre all the size
		// of a pointer (until the end of the struct)
		while (field < (void **)(roomArray + i + 1))
			free(*field++); // and free each memory pointer in that struct
	}

	// finally free the array
	free(roomArray);

	return 0;
}