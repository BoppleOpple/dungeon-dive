#include "roomManip.h"
#include "dungeon.h"
#include "list.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define LINE_BUFFER_SIZE 32
int main(int argc, char *argv[]) {
	char lineBuffer[LINE_BUFFER_SIZE];
	LIST roomList;
	ROOM *dungeon = NULL;
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

	printDungeon(dungeon, &dungeonWidth, &dungeonHeight);

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